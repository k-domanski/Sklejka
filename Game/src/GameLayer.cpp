#include <GameLayer.h>
#include <filesystem>
#include <Scripts/CameraController.h>
#include <Components/NativeScript.h>

using namespace Engine;
GameLayer::GameLayer(): Engine::Layer("Game") {
}

auto GameLayer::OnAttach() -> void {
  LOG_TRACE("Current dir: {}", std::filesystem::current_path().string());
  auto scene = AssetManager::LoadScene("./scenes/gameplay_scene.scene");
  SceneManager::AddScene(scene);
  SceneManager::OpenScene(scene->GetID());

  _player = scene->FindEntity("Player");
  LOG_TRACE("Player name: {}", _player->Name());

  SetupPlayer(_player);
}

auto GameLayer::OnDetach() -> void {
}

auto GameLayer::OnUpdate(double deltaTime) -> void {
  SceneManager::GetCurrentScene()->Update(deltaTime);
  SceneManager::GetCurrentScene()->Draw();
}

auto GameLayer::OnEvent(Engine::Event& event) -> void {
}

auto GameLayer::SetupPlayer(std::shared_ptr< Engine::ECS::Entity >& player) -> void {
  auto scene         = SceneManager::GetCurrentScene();
  auto player_tr     = player->GetComponent< Transform >();
  auto main_camera   = scene->CameraSystem()->MainCamera();
  auto camera_entity = ECS::EntityManager::GetInstance().GetEntity(main_camera->GetEntityID());
  auto camera_tr     = camera_entity->GetComponent< Transform >();
  camera_tr->Position(player_tr->Position());

  auto native_script = camera_entity->AddComponent< Engine::NativeScript >();
  native_script->Attach(std::make_shared< CameraController >(player_tr));
}
