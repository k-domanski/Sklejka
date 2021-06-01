#include <pch.h>
#include "GameManager.h"
#include <Engine.h>

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

GameManager::GameManager() {
  _gameSettings   = std::make_shared< GameSettings >();
  _playerSettings = std::make_shared< PlayerSettings >();
  _soundEngine    = std::shared_ptr< irrklang::ISoundEngine >(irrklang::createIrrKlangDevice());
  _loadingScreen  = std::make_shared< LoadingScreen >();
  _cutscene       = std::make_shared< Cutscene >();
  _mainMenu       = std::make_shared< MainMenu >();
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
      auto scene = Engine::AssetManager::LoadScene("./scenes/_lvl1.scene");
      Engine::SceneManager::AddScene(scene);
      Engine::SceneManager::OpenScene(scene->GetID());
      _instance->CreatePlayer();
      // SetupPlayer(scene);
      break;
    }
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
  _instance->_endLevelMenu->Show("You win. \n Your time was: " + std::to_string(time));
}

auto GameManager::GetCurrentPlayer() -> std::shared_ptr< Engine::ECS::Entity > {
  return _player;
}

auto GameManager::UpdateImpl(float deltaTime) -> void {
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
  /* -=-=-=-=-=-=-=-=- */

  /* Components Setup */
  { /* Player Rect */
    auto transform = player_rect->AddComponent< Transform >();
    auto n0_pos    = node_system->GetNode(0)->GetEntity()->GetComponent< Transform >()->Position();
    auto n1_pos    = node_system->GetNode(1)->GetEntity()->GetComponent< Transform >()->Position();
    transform->Position(n0_pos);
    transform->Scale(glm::vec3(0.9f));
    transform->Forward(glm::normalize(n1_pos - n0_pos));
  }

  { /* Player */
    auto transform = player->AddComponent< Transform >();
    transform->Position({0.0f, 2.0f, 0.0f});
    transform->Euler({180.0f, 0.0f, 180.0f});
  }

  { /* Player Model */
    auto transform = player_model->AddComponent< Transform >();
    transform->Euler({-20.0f, 0.0f, 0.0f});
    auto mesh_renderer = player_model->AddComponent< MeshRenderer >();
    mesh_renderer->SetModel(AssetManager::GetModel("./models/squirrel_anim_idle.fbx"));
    mesh_renderer->SetMaterial(AssetManager::GetMaterial("./materials/animation.mat"));
    auto collider       = player_model->AddComponent< Collider >();
    collider->Size      = {1.2f, 0.35f, 1.9f};
    collider->Center    = {0.0f, 0.0f, -0.35f};
    collider->Type      = Components::ColliderType::Box;
    collider->IsTrigger = true;
    auto rigidbody      = player_model->AddComponent< Rigidbody >();
    rigidbody->SetKinematic(true);
    rigidbody->SetGravity(false);
    auto animator = player_model->AddComponent< Animator >();
    animator->SetAnimation(AssetManager::GetModel("./models/squirrel_anim_idle.fbx"));
  }
  /* -=-=-=-=-=-=-=-=- */

  /* Scene Graph */
  {
    scene_graph->SetParent(player_rect, nullptr);
    scene_graph->SetParent(player, player_rect);
    scene_graph->SetParent(player_model, player);
  }
  /* -=-=-=-=-=- */

  /* Finalize */
  _player     = player;
  _playerRect = player_rect;
  _model      = player_model;
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
  // camera_tr->Position(player_tr->Position());

  auto native_script = _player->AddComponent< Engine::NativeScript >();
  player_tr->Position(glm::vec3{0.0f});
  auto player_controller = native_script->Attach< PlayerController >(player_tr);

  native_script    = _playerRect->AddComponent< Engine::NativeScript >();
  auto player_rect = std::make_shared< PlayerRect >(
      player_controller);  // Save it to variable, because I cannot retrive anything from attached
                           // scripts.......
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
      player_controller);  // Save it to variable, because I cannot retrive anything from attached
                           // scripts.......
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