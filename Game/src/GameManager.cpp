#include <pch.h>
#include "GameManager.h"
#include <Engine.h>

#include "Scripts/CameraController.h"
#include "Scripts/FlightTimer.h"
#include "Scripts/PlayerController.h"
#include "Scripts/PlayerRect.h"
#include "Scripts/StartTimer.h"
#include "Systems/SceneGraph.h"
#include "Components/NativeScript.h"
#include <Scripts/CollisionDetector.h>

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
      SetupPlayer(scene);
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
}

auto GameManager::PlayerSpeedUp() -> void {
  _instance->_speedUpDuration = _instance->GetGameSettings()->PlayerSpeedUpDuration();
  auto fast_speed             = _instance->GetPlayerSettings()->ForwardSpeedBase()
                    * _instance->GetPlayerSettings()->SpeedMultiplier();
  _instance->GetPlayerSettings()->ForwardSpeed(fast_speed);
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

auto GameManager::SetupPlayer(std::shared_ptr< Engine::Scene >& scene) -> void {
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
  auto shadowTarget = std::make_shared< ShadowTarget >(_model->GetID());
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

  // scene->RenderSystem()->SetShadowChecker(shadowTarget);
}