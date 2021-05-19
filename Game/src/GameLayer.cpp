#include <GameLayer.h>
#include <filesystem>
#include <Components/NativeScript.h>
#include <Scripts/CameraController.h>
#include <Scripts/PlayerController.h>
#include <Scripts/PlayerRect.h>

#include "Scripts/ShadowTarget.h"

using namespace Engine;
GameLayer::GameLayer(): Engine::Layer("Game") {
}

auto GameLayer::OnAttach() -> void {
  LOG_TRACE("Current dir: {}", std::filesystem::current_path().string());
  auto scene = AssetManager::LoadScene("./scenes/_lvl1.scene");
  // auto scene = AssetManager::LoadScene("./scenes/_the_map.scene");
  SceneManager::AddScene(scene);
  SceneManager::OpenScene(scene->GetID());

  //_playerRect = scene->FindEntity("Player_Rect");
  //_player = scene->FindEntity("Player");
  // LOG_TRACE("Player name: {}", _player->Name());

  SetupPlayer(scene);
}

auto GameLayer::OnDetach() -> void {
}

auto GameLayer::OnUpdate(double deltaTime) -> void {
  SceneManager::GetCurrentScene()->Update(deltaTime);
  SceneManager::GetCurrentScene()->Draw();
}

auto GameLayer::OnEvent(Engine::Event& event) -> void {
}

auto GameLayer::SetupPlayer(std::shared_ptr< Engine::Scene >& scene) -> void {
  _playerRect = scene->FindEntity("Player_Rect");
  _player     = scene->FindEntity("Player");
  LOG_TRACE("Player name: {}", _player->Name());
  // TODO: Uncomment later :)
  // auto scene         = SceneManager::GetCurrentScene();
  auto player_tr     = _player->GetComponent< Transform >();
  auto main_camera   = scene->CameraSystem()->MainCamera();
  auto camera_entity = ECS::EntityManager::GetInstance().GetEntity(main_camera->GetEntityID());
  auto camera_tr     = camera_entity->GetComponent< Transform >();
  // camera_tr->Position(player_tr->Position());

  auto native_script = _player->AddComponent< Engine::NativeScript >();
  // auto player_controller = native_script->Attach(std::make_shared< PlayerController
  // >(player_tr));
  auto player_controller = native_script->Attach< PlayerController >(player_tr);

  native_script = _playerRect->AddComponent< Engine::NativeScript >();
  native_script->Attach(std::make_shared< PlayerRect >(player_controller));

  native_script = camera_entity->AddComponent< Engine::NativeScript >();
  native_script->Attach(std::make_shared< CameraController >(player_controller));

  native_script = player->AddComponent< Engine::NativeScript >();
  native_script->Attach(std::make_shared< PlayerController >(player_tr));
  auto shadowTarget = std::make_shared< ShadowTarget >(player_tr);
  native_script->Attach(shadowTarget);
  scene->RenderSystem()->SetShadowChecker(shadowTarget);
  glm::mat4(1.0f) * glm::vec4(1.0f);
}
