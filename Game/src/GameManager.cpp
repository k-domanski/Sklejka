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


GameManager::GameManager() {
  _gameSettings   = std::make_shared< GameSettings >();
  _playerSettings = std::make_shared< PlayerSettings >();
  _soundEngine    = std::shared_ptr< irrklang::ISoundEngine >(irrklang::createIrrKlangDevice());
  _loadingScreen  = std::make_shared< LoadingScreen >();
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

auto GameManager::SwitchScene(SceneName scene) -> void {
  switch (scene) {
    case SceneName::MainMenu: {
      break;
    }
    case SceneName::Loading: {
      ShowLoadingScreen();
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

auto GameManager::GetSoundEngine() noexcept -> std::shared_ptr< irrklang::ISoundEngine > {
  return _instance->_soundEngine;
}

auto GameManager::SetupPlayer(std::shared_ptr< Engine::Scene >& scene) -> void {
  _playerRect = scene->FindEntity("Player_Rect");
  _player     = scene->FindEntity("Player");
  if (_player == nullptr || _playerRect == nullptr) {
    LOG_TRACE("No player entity found");
    return;
  }
  LOG_TRACE("Player name: {}", _player->Name());
  // TODO: Uncomment later :)
  // auto scene         = SceneManager::GetCurrentScene();
  auto player_tr     = _player->GetComponent< Engine::Transform >();
  auto sceneGraph    = scene->SceneGraph();
  auto model         = sceneGraph->GetChildren(_player->GetID());
  auto main_camera   = scene->CameraSystem()->MainCamera();
  auto camera_entity = Engine::ECS::EntityManager::GetInstance().GetEntity(main_camera->GetEntityID());
  auto camera_tr     = camera_entity->GetComponent< Engine::Transform >();
  // camera_tr->Position(player_tr->Position());

  auto native_script = _player->AddComponent< Engine::NativeScript >();
  // auto player_controller = native_script->Attach(std::make_shared< PlayerController
  // >(player_tr));
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
  auto shadowTarget = std::make_shared< ShadowTarget >(model[0]);
  native_script->Attach(shadowTarget);
  auto flightTimer =
      std::make_shared< FlightTimer >();  // Save it to variable, because I cannot retrive anything
                                          // from attached scripts.......
  flightTimer->CanCount(false);
  native_script->Attach(flightTimer);

  // native_script->Attach(std::make_shared< StartTimer >(player_rect, flightTimer));
  auto start_timer = native_script->Attach< StartTimer >(player_rect, flightTimer);
  start_timer->CanCount(true);

  // scene->RenderSystem()->SetShadowChecker(shadowTarget);
}