#include <pch.h>
#include "GameManager.h"
#include <Engine.h>
#include <iomanip>

#include "Scripts/CameraController.h"
#include "Scripts/FlightTimer.h"
#include "Scripts/PlayerController.h"
#include "Scripts/PlayerRect.h"
#include "Scripts/StartTimer.h"
#include "Scripts/BellThrower.h"
#include "Systems/SceneGraph.h"
#include "Components/NativeScript.h"
#include <Scripts/CollisionDetector.h>
#include "Components/Rigidbody.h"
#include "Components/Animator.h"
#include "Systems/NodeSystem.h"

using namespace Engine;
using namespace Engine::Components;
using namespace Engine::ECS;

GameManager::GameManager() {
  _gameSettings   = std::make_shared< GameSettings >();
  _playerSettings = std::make_shared< PlayerSettings >();
  _soundEngine    = std::shared_ptr< irrklang::ISoundEngine >(irrklang::createIrrKlangDevice());
  _loadingScreen  = std::make_shared< LoadingScreen >();
  _cutscene       = std::make_shared< Cutscene >();
  _mainMenu       = std::make_shared< MainMenu >();
  _levelSelection = std::make_shared< LevelSelection >();
  _options        = std::make_shared< OptionsMenu >();
  _fishEyeShader  = Engine::AssetManager::GetShader("./shaders/fish_eye.glsl");
}

auto GameManager::Initialize() -> void {
  if (_instance == nullptr) {
    _instance = std::shared_ptr< GameManager >(new GameManager());
  }
}

auto GameManager::GetGameSettings() noexcept -> std::shared_ptr< GameSettings > {
  return _instance->_gameSettings;
}

auto GameManager::GetPlayerSettings() noexcept -> std::shared_ptr< PlayerSettings > {
  return _instance->_playerSettings;
}

auto GameManager::GetSoundEngine() noexcept -> std::shared_ptr< irrklang::ISoundEngine > {
  return _instance->_soundEngine;
}

auto GameManager::SwitchScene(SceneName scene) -> void {
  _instance->_currentSceneName = scene;
  if (_instance->_pauseMenu != nullptr)
    _instance->_pauseMenu = nullptr;
  switch (scene) {
    case SceneName::MainMenu: {
      Engine::SceneManager::OpenScene(_instance->_mainMenu->Scene()->GetID());
      break;
    }
    case SceneName::Loading: {
      ShowLoadingScreen();
      break;
    }
    case SceneName::Cutscene: {
      _instance->PlayCutscene();
      break;
    }
    case SceneName::LVL_1: {
      // auto scene = Engine::AssetManager::LoadScene("./scenes/_lvl1.scene");
      auto scene = Engine::AssetManager::LoadScene("./scenes/LEVEL_1.scene");
      // auto scene = Engine::AssetManager::LoadScene("./scenes/LEVEL_1_BOSSNODES.scene");
      Engine::SceneManager::AddScene(scene);
      Engine::SceneManager::OpenScene(scene->GetID());
      _instance->NextFrameTrigger();
      break;
    }
    case SceneName::LevelSelection:
      Engine::SceneManager::OpenScene(_instance->_levelSelection->Scene()->GetID());
      break;
    case SceneName::Options:
      Engine::SceneManager::OpenScene(_instance->_options->Scene()->GetID());
      break;
  }
}

auto GameManager::GetScene(SceneName scene) -> std::shared_ptr< Engine::Scene > {
  if (scene == _instance->_currentSceneName) {
    return Engine::SceneManager::GetCurrentScene();
  }
  switch (scene) {
    case SceneName::LVL_1: {
      return Engine::AssetManager::LoadScene("./scenes/_lvl1.scene");
    }
  }
}

auto GameManager::GetNextSceneName() -> SceneName {
  auto it = _instance->_currentSceneName._to_index();
  ++it %= SceneName::_size();
  return SceneName::_from_index(it);
}

auto GameManager::ShowLoadingScreen() -> void {
  Engine::SceneManager::OpenScene(_instance->_loadingScreen->Scene()->GetID());
}

auto GameManager::Update(float deltaTime) -> void {
  _instance->UpdateImpl(deltaTime);

  // test pause menu:
  if (Engine::Input::IsKeyPressed(Engine::Key::ESCAPE)) {
    _instance->_pauseMenu->Show();
  }
}

auto GameManager::PlayerSpeedUp() -> void {
  _instance->_speedUpDuration = _instance->GetGameSettings()->PlayerSpeedUpDuration();
  auto fast_speed             = _instance->GetPlayerSettings()->ForwardSpeedBase()
                    * _instance->GetPlayerSettings()->SpeedMultiplier();
  _instance->GetPlayerSettings()->ForwardSpeed(fast_speed);
}

auto GameManager::ShowLevelSumUp(float time, bool win) -> void {
  std::stringstream s;
  s << "Your time was: " << std::fixed << std::setprecision(2) << time;
  _instance->_endLevelMenu->Show(s.str(), "You win");
}

auto GameManager::GetCurrentPlayer() -> std::shared_ptr< Engine::ECS::Entity > {
  return _player;
}

auto GameManager::KillPlayer() -> void {
  _instance->KillPlayerImpl();
}

auto GameManager::UpdateImpl(float deltaTime) -> void {
#if defined(_DEBUG)
  if (Input::IsKeyPressed(Key::K)) {
    auto folder = AssetManager::GetAssetsFolders().scenes;
    AssetManager::SaveScene(SceneManager::GetCurrentScene(), folder + "__LEVEL__DUMP__.scene");
  }
  if (Input::IsKeyPressed(Key::P)) {
    ShowLevelSumUp(21.37f, true);
  }
#endif

  if (_frameWaitCounter > 0) {
    if (_frameWaitCounter == 1) {
      switch (_currentSceneName) {
        case SceneName::LVL_1: {
          /* This is delayed 1 frame */
          _instance->CreatePlayer();
          break;
        }
      }
    }
    --_frameWaitCounter;
  }

  if (_speedUpDuration > 0.0f) {
    _speedUpDuration -= deltaTime;
  } else {
    GetPlayerSettings()->ForwardSpeed(GetPlayerSettings()->ForwardSpeedBase());
  }

  auto base_speed    = GetPlayerSettings()->ForwardSpeedBase();
  auto max_speed     = base_speed * GetPlayerSettings()->SpeedMultiplier();
  auto current_speed = GetPlayerSettings()->ForwardSpeed();

  auto factor = (current_speed - base_speed) / max_speed;
  _fishEyeShader->SetValue("u_Factor", factor);
}

auto GameManager::PlayCutscene() -> void {
  _cutscene->Reset();
  Engine::SceneManager::OpenScene(_cutscene->Scene()->GetID());
}

auto GameManager::CreatePlayer() -> void {
  using Components::Collider;
  using Components::MeshRenderer;
  using Components::Rigidbody;
  using Engine::ECS::Entity;
  using Engine::ECS::EntityManager;
  auto& entity_manager = EntityManager::GetInstance();
  auto& scene_graph    = SceneManager::GetCurrentScene()->SceneGraph();
  auto& node_system    = SceneManager::GetCurrentScene()->NodeSystem();

  /* Create Entities */
  auto player_rect = entity_manager.CreateEntity();
  player_rect->Name("Player_Rect");
  auto player = entity_manager.CreateEntity();
  player->Name("Player");
  auto player_model = entity_manager.CreateEntity();
  player_model->Name("Player_Model");
  auto main_camera = entity_manager.CreateEntity();
  main_camera->Name("Main_Camera");
  /* -=-=-=-=-=-=-=-=- */

  // scene_graph->Update(-1);

  /* Components Setup */
  { /* Player Rect */
    auto transform = player_rect->AddComponent< Transform >();
    /* Skip 1st node */
    auto n1_pos = node_system->GetNode(0, NodeTag::Player)
                      ->GetEntity()
                      ->GetComponent< Transform >()
                      ->WorldPosition();
    auto n2_pos = node_system->GetNode(1, NodeTag::Player)
                      ->GetEntity()
                      ->GetComponent< Transform >()
                      ->WorldPosition();
    transform->Position(n1_pos);
    transform->Scale(glm::vec3(0.9f));
    transform->Forward(glm::normalize(n2_pos - n1_pos));
  }

  { /* Player */
    auto transform = player->AddComponent< Transform >();
    transform->Position({0.0f, 2.0f, 0.0f});
    transform->Euler(glm::radians(glm::vec3{180.0f, 0.0f, 180.0f}));
  }

  { /* Player Model */
    auto transform = player_model->AddComponent< Transform >();
    transform->Euler(glm::radians(glm::vec3{-20.0f, 0.0f, 0.0f}));
    auto mesh_renderer = player_model->AddComponent< MeshRenderer >();
    mesh_renderer->SetModel(AssetManager::GetModel("./models/squirrel_anim_idle.fbx"));
    mesh_renderer->SetMaterial(AssetManager::GetMaterial("./materials/animation.mat"));
    auto collider       = player_model->AddComponent< Collider >();
    collider->Size      = {1.1f, 0.10f, 1.4f};
    collider->Center    = {0.0f, 0.0f, -0.35f};
    collider->Type      = Components::ColliderType::Box;
    collider->IsTrigger = true;
    auto rigidbody      = player_model->AddComponent< Rigidbody >();
    rigidbody->SetKinematic(true);
    rigidbody->SetGravity(false);
    auto animator = player_model->AddComponent< Animator >();
    animator->SetAnimation(AssetManager::GetModel("./models/squirrel_anim_idle.fbx"));
  }

  { /* Camera */
    auto transform = main_camera->AddComponent< Transform >();
    auto camera    = main_camera->AddComponent< Camera >();
    camera->Fov(70.0f);
    camera->flags.Set(CameraFlag::MainCamera);
    camera->NearPlane(0.01f);
    camera->FarPlane(1000.0f);
  }
  /* -=-=-=-=-=-=-=-=- */

  /* Scene Graph */
  {
    scene_graph->SetParent(main_camera, nullptr);
    scene_graph->SetParent(player_rect, nullptr);
    scene_graph->SetParent(player, player_rect);
    scene_graph->SetParent(player_model, player);
  }
  /* -=-=-=-=-=- */

  /* Finalize */
  player_model->layer.SetState(LayerMask::Flag::Player);
  player_model->collisionLayer.SetState(LayerMask::Flag::Default);
  _player     = player;
  _playerRect = player_rect;
  _model      = player_model;

  SceneManager::GetCurrentScene()->CameraSystem()->FindMainCamera();
  SetupScripts();
  /* -=-=-=-=- */
}

auto GameManager::SetupScripts() -> void {
  auto scene = SceneManager::GetCurrentScene();

  if (_player == nullptr || _playerRect == nullptr) {
    LOG_TRACE("No player entity found");
    return;
  }
  LOG_TRACE("Player name: {}", _player->Name());
  // TODO: Uncomment later :)
  // auto scene         = SceneManager::GetCurrentScene();
  auto player_tr   = _player->GetComponent< Engine::Transform >();
  auto sceneGraph  = scene->SceneGraph();
  auto main_camera = scene->CameraSystem()->MainCamera();
  auto camera_entity =
      Engine::ECS::EntityManager::GetInstance().GetEntity(main_camera->GetEntityID());
  auto camera_tr = camera_entity->GetComponent< Engine::Transform >();
  camera_tr->Position(player_tr->Position());

  auto native_script = _player->AddComponent< Engine::NativeScript >();
  player_tr->Position(glm::vec3{0.0f});
  auto player_controller = native_script->Attach< PlayerController >(player_tr);

  native_script    = _playerRect->AddComponent< Engine::NativeScript >();
  auto player_rect = std::make_shared< PlayerRect >(
      player_controller,
      _model->GetComponent< Transform >());  // Save it to variable, because I cannot retrive
                                             // anything from attached scripts.......
  player_rect->CanMove(false);
  native_script->Attach(player_rect);

  native_script = camera_entity->AddComponent< Engine::NativeScript >();
  native_script->Attach(std::make_shared< CameraController >(player_controller));

  native_script->Attach(std::make_shared< PlayerController >(player_tr));
  auto shadowTarget = std::make_shared< ShadowTarget >(_model);
  native_script->Attach(shadowTarget);
  auto flightTimer =
      std::make_shared< FlightTimer >();  // Save it to variable, because I cannot retrive anything
                                          // from attached scripts.......
  flightTimer->CanCount(false);
  native_script->Attach(flightTimer);

  // native_script->Attach(std::make_shared< StartTimer >(player_rect, flightTimer));
  auto start_timer = native_script->Attach< StartTimer >(player_rect, flightTimer);
  start_timer->CanCount(true);

  native_script = _model->AddComponent< Engine::NativeScript >();
  native_script->Attach< CollisionDetector >();

  native_script->Attach< BellThrower >();

  _instance->_pauseMenu    = std::make_shared< PauseMenu >();
  _instance->_endLevelMenu = std::make_shared< EndLevelMenu >();
  // scene->RenderSystem()->SetShadowChecker(shadowTarget);
}

auto GameManager::NextFrameTrigger() -> void {
  _frameWaitCounter = 2;
}

auto GameManager::KillPlayerImpl() -> void {
  auto player_rect = _playerRect->GetComponent< NativeScript >()->GetScript< PlayerRect >();
  player_rect->CanMove(false);
  player_rect->Enable(false);

  auto collider       = _model->GetComponent< Collider >();
  collider->IsTrigger = false;
  auto rb             = _model->GetComponent< Rigidbody >();

  rb->SetGravity(true);
  rb->SetKinematic(false);

  auto camera = SceneManager::GetCurrentScene()->CameraSystem()->MainCamera();
  auto timer  = camera->GetEntity()->GetComponent< NativeScript >()->GetScript< FlightTimer >();
  auto time   = timer->GetTime();
  timer->CanCount(false);

  ShowLevelSumUp(time, false);
}

/* Try not to use this shit anymore */
auto GameManager::SetupPlayer(std::shared_ptr< Engine::Scene >& scene) -> void {
  return;
  _playerRect = scene->FindEntity("Player_Rect");
  _player     = scene->FindEntity("Player");
  _model      = scene->FindEntity("Model");

  if (_player == nullptr || _playerRect == nullptr) {
    LOG_TRACE("No player entity found");
    return;
  }
  LOG_TRACE("Player name: {}", _player->Name());
  // TODO: Uncomment later :)
  // auto scene         = SceneManager::GetCurrentScene();
  auto player_tr   = _player->GetComponent< Engine::Transform >();
  auto sceneGraph  = scene->SceneGraph();
  auto main_camera = scene->CameraSystem()->MainCamera();
  auto camera_entity =
      Engine::ECS::EntityManager::GetInstance().GetEntity(main_camera->GetEntityID());
  auto camera_tr = camera_entity->GetComponent< Engine::Transform >();
  // camera_tr->Position(player_tr->Position());

  auto native_script = _player->AddComponent< Engine::NativeScript >();
  player_tr->Position(glm::vec3{0.0f});
  auto player_controller = native_script->Attach< PlayerController >(player_tr);

  native_script    = _playerRect->AddComponent< Engine::NativeScript >();
  auto player_rect = std::make_shared< PlayerRect >(
      player_controller,
      _model->GetComponent< Transform >());  // Save it to variable, because I cannot retrive
                                             // anything from attached scripts.......
  player_rect->CanMove(false);
  native_script->Attach(player_rect);

  native_script = camera_entity->AddComponent< Engine::NativeScript >();
  native_script->Attach(std::make_shared< CameraController >(player_controller));

  native_script->Attach(std::make_shared< PlayerController >(player_tr));
  auto shadowTarget = std::make_shared< ShadowTarget >(_model);
  native_script->Attach(shadowTarget);
  auto flightTimer =
      std::make_shared< FlightTimer >();  // Save it to variable, because I cannot retrive anything
                                          // from attached scripts.......
  flightTimer->CanCount(false);
  native_script->Attach(flightTimer);

  // native_script->Attach(std::make_shared< StartTimer >(player_rect, flightTimer));
  auto start_timer = native_script->Attach< StartTimer >(player_rect, flightTimer);
  start_timer->CanCount(true);

  native_script = _model->AddComponent< Engine::NativeScript >();
  native_script->Attach< CollisionDetector >();

  native_script->Attach< BellThrower >();

  _instance->_pauseMenu    = std::make_shared< PauseMenu >();
  _instance->_endLevelMenu = std::make_shared< EndLevelMenu >();
  // scene->RenderSystem()->SetShadowChecker(shadowTarget);
}