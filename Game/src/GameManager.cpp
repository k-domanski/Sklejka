#include <pch.h>
#include "GameManager.h"
#include <Engine.h>
#include <iomanip>
#include <regex>
#include <limits>

#include "Scripts/CameraController.h"
#include "Scripts/FlightTimer.h"
#include "Scripts/PlayerController.h"
#include "Scripts/PlayerRect.h"
#include "Scripts/StartTimer.h"
#include "Scripts/AcornThrower.h"
#include "Systems/SceneGraph.h"
#include "Components/NativeScript.h"
#include <Scripts/CollisionDetector.h>
#include "Components/Rigidbody.h"
#include "Components/Animator.h"
#include "Scripts/Boss.h"
#include "Systems/NodeSystem.h"
#include "Components/ParticleEmitter.h"

using namespace Engine;
using namespace Engine::Components;
using namespace Engine::ECS;

GameManager::GameManager() {
  _gameSettings   = std::make_shared< GameSettings >();
  _playerSettings = std::make_shared< PlayerSettings >();
  _soundEngine    = std::shared_ptr< irrklang::ISoundEngine >(irrklang::createIrrKlangDevice());
  _cutscene       = std::make_shared< Cutscene >();
  _mainMenu       = std::make_shared< MainMenu >();
  _levelSelection = std::make_shared< LevelSelection >();
  _options        = std::make_shared< OptionsMenu >();
  _fishEyeShader  = AssetManager::GetShader("./shaders/fish_eye.glsl");
  _bellOutlineMaterial = AssetManager::GetMaterial("materials/bell_outline.mat");
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

  _playerRect = nullptr;
  _player     = nullptr;
  _model      = nullptr;
  switch (scene) {
    case SceneName::MainMenu: {
      Engine::SceneManager::OpenScene(_instance->_mainMenu->Scene()->GetID());
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
    case SceneName::LevelSelection: {
      Engine::SceneManager::OpenScene(_instance->_levelSelection->Scene()->GetID());
      break;
    }
    case SceneName::Options: {
      Engine::SceneManager::OpenScene(_instance->_options->Scene()->GetID());
      break;
    }

      if (IsGameplayScene()) {
        FindBells();
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

auto GameManager::GetNextSceneName() -> SceneName {
  auto it = _instance->_currentSceneName._to_index();
  ++it %= SceneName::_size();
  return SceneName::_from_index(it);
}

auto GameManager::GetCurrentSceneName() -> SceneName {
  return _instance->_currentSceneName;
}

auto GameManager::Update(float deltaTime) -> void {
  _instance->UpdateImpl(deltaTime);

  // test pause menu:
  if (Engine::Input::IsKeyPressed(Engine::Key::ESCAPE)) {
    if (_instance->_pauseMenu != nullptr)
      _instance->_pauseMenu->Show();
  }
}

auto GameManager::PlayerSpeedUp() -> void {
  _instance->_speedUpDuration = _instance->GetGameSettings()->PlayerSpeedUpDuration();
  auto fast_speed             = _instance->GetPlayerSettings()->ForwardSpeedBase()
                    * _instance->GetPlayerSettings()->SpeedMultiplier();
  _instance->GetPlayerSettings()->ForwardSpeed(fast_speed);
}

auto GameManager::ShowLevelSumUp(bool win, float time, int bells) -> void {
  _instance->_endLevelMenu->Show(win, time, bells);
}

auto GameManager::GetCurrentPlayer() -> std::shared_ptr< Engine::ECS::Entity > {
  return _player;
}

auto GameManager::KillPlayer() -> void {
  _instance->KillPlayerImpl();
}

auto GameManager::Win() -> void {
  _instance->WinImpl();
}

auto GameManager::IsGameplayState() -> bool {
  const auto is_gameplay_scene = IsGameplayScene();
  const auto is_initialized    = IsGameplayInitialized();

  return is_gameplay_scene && is_initialized;
}

auto GameManager::IsGameplayScene() -> bool {
  const auto is_gameplay_scene = (_instance->_currentSceneName == +SceneName::LVL_1);
  return is_gameplay_scene;
}

auto GameManager::IsGameplayInitialized() -> bool {
  const auto is_initialized = (_playerRect != nullptr && _player != nullptr && _model != nullptr);
  return is_initialized;
}

auto GameManager::UpdateImpl(float deltaTime) -> void {
#if defined(_DEBUG)
  if (Input::IsKeyPressed(Key::K)) {
    auto folder = AssetManager::GetAssetsFolders().scenes;
    AssetManager::SaveScene(SceneManager::GetCurrentScene(), folder + "__LEVEL__DUMP__.scene");
  }
#endif

  if (_frameWaitCounter > 0) {
    if (_frameWaitCounter == 1) {
      switch (_currentSceneName) {
        case SceneName::LVL_1: {
          /* This is delayed 1 frame */
          _instance->CreatePlayer();
          _instance->CreateBoss();
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

  if (IsGameplayState()) {
    UpdateMarkerColor();
  }
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
  auto emitter_left = entity_manager.CreateEntity();
  emitter_left->Name("Trail Left");
  auto emitter_right = entity_manager.CreateEntity();
  emitter_left->Name("Trail Right");
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

  {
    /* Emitters */
    auto em_l     = emitter_left->AddComponent< ParticleEmitter >(100);
    auto em_r     = emitter_right->AddComponent< ParticleEmitter >(100);
    auto material = AssetManager::GetMaterial("materials/trail.mat");
    em_l->Material(material);
    em_r->Material(material);

    em_l->EmitCount(100);
    em_r->EmitCount(100);

    constexpr auto scale    = 0.2f;
    constexpr auto lifetime = 0.75f;
    constexpr auto decay    = scale / lifetime;

    em_l->Scale(glm::vec2(scale));
    em_r->Scale(glm::vec2(scale));

    em_l->Lifetime(lifetime);
    em_r->Lifetime(lifetime);

    em_l->SizeDecay(decay);
    em_r->SizeDecay(decay);

    em_l->SpawnRate(50.0f);
    em_r->SpawnRate(50.0f);

    auto tr_l = emitter_left->AddComponent< Transform >();
    auto tr_r = emitter_right->AddComponent< Transform >();

    constexpr auto x = 0.70f;
    constexpr auto y = -0.1f;
    constexpr auto z = -0.55f;
    tr_l->Position({-x, y, z});
    tr_r->Position({x, y, z});
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
    scene_graph->SetParent(emitter_left, player_model);
    scene_graph->SetParent(emitter_right, player_model);
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

auto GameManager::CreateBoss() -> void {
  auto& entity_manager = EntityManager::GetInstance();
  auto& scene_graph    = SceneManager::GetCurrentScene()->SceneGraph();
  auto& node_system    = SceneManager::GetCurrentScene()->NodeSystem();

  auto boss = entity_manager.CreateEntity();
  boss->LoadFromJson("./Assets/prefabs/boss.prefab");
  auto weasel = entity_manager.CreateEntity();
  weasel->LoadFromJson("./Assets/prefabs/weasel.prefab");
  auto boss_jet = entity_manager.CreateEntity();
  boss_jet->LoadFromJson("./Assets/prefabs/jet.prefab");

  scene_graph->SetParent(weasel, boss);
  scene_graph->SetParent(boss_jet, weasel);

  auto transform = boss->GetComponent< Transform >();
  /* Skip 1st node */
  auto n1_pos = node_system->GetNode(0, NodeTag::Boss)
                    ->GetEntity()
                    ->GetComponent< Transform >()
                    ->WorldPosition();
  auto n2_pos = node_system->GetNode(1, NodeTag::Boss)
                    ->GetEntity()
                    ->GetComponent< Transform >()
                    ->WorldPosition();
  transform->Position(n1_pos);
  transform->Forward(glm::normalize(n2_pos - n1_pos));

  auto boss_native_script = boss->AddComponent< NativeScript >();
  boss_native_script->Attach(std::make_shared< Boss >(
      _playerRect->GetComponent< NativeScript >()->GetScript< PlayerRect >()));
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
  auto start_timer = native_script->Attach< StartTimer >(player_rect, flightTimer, shadowTarget);
  start_timer->CanCount(true);

  native_script = _model->AddComponent< Engine::NativeScript >();
  native_script->Attach< CollisionDetector >();

  native_script->Attach< AcornThrower >();

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

  ShowLevelSumUp(false, 0, 0);
}

auto GameManager::WinImpl() -> void {
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

  ShowLevelSumUp(true, time, 10);  // TODO: get hitted bells value
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
  auto start_timer = native_script->Attach< StartTimer >(player_rect, flightTimer, shadowTarget);
  start_timer->CanCount(true);

  native_script = _model->AddComponent< Engine::NativeScript >();
  native_script->Attach< CollisionDetector >();

  native_script->Attach< AcornThrower >();

  _instance->_pauseMenu    = std::make_shared< PauseMenu >();
  _instance->_endLevelMenu = std::make_shared< EndLevelMenu >();
  // scene->RenderSystem()->SetShadowChecker(shadowTarget);
}

auto GameManager::FindBells() -> void {
  auto entities = SceneManager::GetCurrentScene()->Entities();
  _bellsTransform.clear();
  for (const auto& ent : entities) {
    const std::regex rx("BELL.*");
    if (!std::regex_match(ent->Name(), rx)) {
      continue;
    }
    _bellsTransform.push_back(ent->GetComponent< Transform >());
  }
}

auto GameManager::UpdateMarkerColor() -> void {
  float closest2       = std::numeric_limits< float >::max();
  const auto& model_tr = _model->GetComponent< Transform >();
  for (const auto& tr : _bellsTransform) {
    auto d2  = glm::distance2(tr->WorldPosition(), model_tr->WorldPosition());
    closest2 = glm::min(d2, closest2);
  }
  auto closest    = glm::sqrt(closest2);
  auto throw_dist = _instance->_playerSettings->ThrowDistance();
  /* Get ratio with remapping into [-1, 1] range */
  auto ratio = glm::clamp((closest / throw_dist) - 1.0f, -1.0f, 1.0f);
  _instance->_bellOutlineMaterial->GetShader()->SetValue("u_Ratio", ratio);
}
