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
  stbi_set_flip_vertically_on_load(true);
  _image->Texture(AssetManager::GetTexture2D("./textures/Loading.png"));
  stbi_set_flip_vertically_on_load(false);
  auto size = Window::Get().GetScreenSize();
  _image->Size(size);

  _entity          = ECS::EntityManager::GetInstance().CreateEntity();
  auto ui_renderer = _entity->AddComponent< Components::UIRenderer >();
  auto tr          = _entity->AddComponent< Transform >();
  tr->Position(glm::vec3(size * 0.5f, 0.0f));
  ui_renderer->GetElements().push_back(_image);

  if (current_scene != nullptr) {
    SceneManager::OpenScene(current_scene->GetID());
  }
}

auto LoadingScreen::Scene() -> std::shared_ptr< Engine::Scene > {
  return _scene;
}
