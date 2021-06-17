#include "Cutscene.h"
#include "Engine.h"
#include "GameManager.h"
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
  auto window_size = glm::vec2(1600.0f, 900.0f);  // refered size  // Window::Get().GetScreenSize();
  transform->Position(glm::vec3(window_size * 0.5f, 0.0f));

  _image = std::make_shared< Renderer::Image >();
  stbi_set_flip_vertically_on_load(true);
  _image->Texture(AssetManager::GetTexture2D("./textures/cutscene.png"));
  _image->Size(window_size);

  ui_renderer->GetElements().push_back(_image);

  _skipButton = std::make_shared< Renderer::Button >();
  _skipButton->Background(Engine::AssetManager::GetTexture2D("./textures/UI/empty.png"));
  _skipButton->Size({165.0f, 65.0f});
  _skipButton->HandleSize({165.0f, 65.0f});
  _skipButton->Offset({650.0f, -400.0f});
  _skipButton->Color({0.6f, 0.6f, 0.6f, 1.0f});
  _skipButton->SelectedColor(glm::vec4(1.0f));
  _skipButton->text("SKIP");
  _skipButton->TextColor({0.0f, 0.0f, 0.0f, 1.0f});
  _skipButton->TextOffset({-60.0f, -20.0f});
  _skipButton->OnPress([this]() {
    Reset();
    GameManager::SwitchScene(SceneName::LVL_1);
  });
  _skipButton->BackgroundShader(
      AssetManager::GetShader("./shaders/bar.glsl"));  // HACK: to repair offset on button
  stbi_set_flip_vertically_on_load(false);

  ui_renderer->AddElement(_skipButton);
  ui_renderer->AddButton(_skipButton);

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