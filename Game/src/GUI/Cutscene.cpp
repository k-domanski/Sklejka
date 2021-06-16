#include "Cutscene.h"
#include "Engine.h"
#include "Components/UIRenderer.h"
#include "Components/NativeScript.h"
#include "Scripts/CutscenePlayer.h"

using namespace Engine;
using namespace Engine::ECS;

Cutscene::Cutscene() {
  _scene             = std::make_shared< Engine::Scene >(12345);
  auto current_scene = SceneManager::GetCurrentScene();

  SceneManager::AddScene(_scene);
  SceneManager::OpenScene(_scene->GetID());

  auto entity      = EntityManager::GetInstance().CreateEntity();
  auto ui_renderer = entity->AddComponent< Components::UIRenderer >();
  auto transform   = entity->AddComponent< Transform >();
  auto window_size = glm::vec2(1600.0f, 900.0f);//refered size  // Window::Get().GetScreenSize();
  transform->Position(glm::vec3(window_size * 0.5f, 0.0f));

  _image = std::make_shared< Renderer::Image >();
  stbi_set_flip_vertically_on_load(true);
  _image->Texture(AssetManager::GetTexture2D("./textures/cutscene.png"));
  stbi_set_flip_vertically_on_load(false);
  _image->Size(window_size);

  ui_renderer->GetElements().push_back(_image);

  auto native_script = entity->AddComponent< NativeScript >();
  _cutscenePlayer    = native_script->Attach< CutscenePlayer >(_image->Shader());

  SceneManager::GetCurrentScene()->OnWindowResize(Window::Get().GetScreenSize());

  if (current_scene != nullptr) {
    SceneManager::OpenScene(current_scene->GetID());
  }
}

auto Cutscene::Scene() -> std::shared_ptr< Engine::Scene > {
  return _scene;
}

auto Cutscene::Reset() -> void {
  _image->Shader()->SetVector("u_Tiling", glm::vec2(1.0f, 1.0f));
  _image->Shader()->SetVector("u_Offset", glm::vec2(0.0f, 0.0f));
  _cutscenePlayer->Reset();
}