#include "LoadingScreen.h"
#include "Components/UIRenderer.h"

using namespace Engine;
using namespace Engine::Renderer;

LoadingScreen::LoadingScreen() {
  _scene             = std::make_shared< Engine::Scene >(1337);
  auto current_scene = SceneManager::GetCurrentScene();
  SceneManager::AddScene(_scene);
  SceneManager::OpenScene(_scene->GetID());

  _image    = std::make_shared< Image >();
  auto size = Window::Get().GetScreenSize();
  _image->Size(size);

  _entity          = ECS::EntityManager::GetInstance().CreateEntity();
  auto ui_renderer = _entity->AddComponent< Components::UIRenderer >();
  _entity->AddComponent< Transform >();
  ui_renderer->GetElements().push_back(_image);

  if (current_scene != nullptr) {
    SceneManager::OpenScene(current_scene->GetID());
  }
}

auto LoadingScreen::Scene() -> std::shared_ptr< Engine::Scene > {
  return _scene;
}
