#include "LoadingScreen.h"
#include "Components/UIRenderer.h"

using namespace Engine;
using namespace Engine::Renderer;

LoadingScreen::LoadingScreen() {
  _scene             = std::make_shared< Engine::Scene >(1337);
  auto current_scene = SceneManager::GetCurrentScene();
  SceneManager::AddScene(_scene);
  SceneManager::OpenScene(_scene->GetID());

  _image = std::make_shared< Image >();
  _image->Texture(AssetManager::GetTexture2D("./textures/Loading.png"));
  auto size = Window::Get().GetScreenSize();
  _image->Size(size);

  _entity          = ECS::EntityManager::GetInstance().CreateEntity();
  auto ui_renderer = _entity->AddComponent< Components::UIRenderer >();
  auto tr          = _entity->AddComponent< Transform >();
  tr->Position({0.0f, 0.0f, 0.0f});
  ui_renderer->GetElements().push_back(_image);

  if (current_scene != nullptr) {
    SceneManager::OpenScene(current_scene->GetID());
  }
}

auto LoadingScreen::Scene() -> std::shared_ptr< Engine::Scene > {
  return _scene;
}
