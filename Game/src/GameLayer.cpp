#include <GameLayer.h>
#include <filesystem>
#include <Components/NativeScript.h>
#include <Scripts/CameraController.h>
#include <Scripts/PlayerController.h>
#include <Scripts/PlayerRect.h>
#include <Systems/SceneGraph.h>

#include <Scripts/ShadowTarget.h>
#include <Scripts/FlightTimer.h>

#include <GameManager.h>
#include "Scripts/StartTimer.h"

using namespace Engine;
GameLayer::GameLayer(): Engine::Layer("Game") {
}

auto GameLayer::OnAttach() -> void {
  LOG_TRACE("Current dir: {}", std::filesystem::current_path().string());
  GameManager::SwitchScene(SceneName::Loading);
  GameManager::SwitchScene(SceneName::LVL_1);

  auto scene = SceneManager::GetCurrentScene();
  SetupPlayer(scene);

  // GameManager::ShowLoadingScreen();
}

auto GameLayer::OnDetach() -> void {
}

auto GameLayer::OnUpdate(double deltaTime) -> void {
  if (Input::IsKeyPressed(Key::D1)) {
    GameManager::SwitchScene(SceneName::Loading);
  } else if (Input::IsKeyPressed(Key::D2)) {
    GameManager::SwitchScene(SceneName::LVL_1);
  }

  SceneManager::GetCurrentScene()->Update(deltaTime);
  SceneManager::GetCurrentScene()->Draw();
}

auto GameLayer::OnEvent(Engine::Event& event) -> void {
}

auto GameLayer::SetupPlayer(std::shared_ptr< Engine::Scene >& scene) -> void {
  _playerRect = scene->FindEntity("Player_Rect");
  _player     = scene->FindEntity("Player");
  if (_player == nullptr || _playerRect == nullptr) {
    LOG_TRACE("No player entity found");
    return;
  }
  LOG_TRACE("Player name: {}", _player->Name());
  // TODO: Uncomment later :)
  // auto scene         = SceneManager::GetCurrentScene();
  auto player_tr     = _player->GetComponent< Transform >();
  auto sceneGraph    = scene->SceneGraph();
  auto model         = sceneGraph->GetChildren(_player->GetID());
  auto main_camera   = scene->CameraSystem()->MainCamera();
  auto camera_entity = ECS::EntityManager::GetInstance().GetEntity(main_camera->GetEntityID());
  auto camera_tr     = camera_entity->GetComponent< Transform >();
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

  native_script->Attach(std::make_shared< StartTimer >(player_rect, flightTimer));

  // scene->RenderSystem()->SetShadowChecker(shadowTarget);
}
