#include "CreditsMenu.h"

#include "GameManager.h"
#include "Components/UIRenderer.h"
#include "Engine/SceneManager.h"
#include "ECS/EntityManager.h"

CreditsMenu::CreditsMenu() {
  _scene             = std::make_shared< Engine::Scene >(691111111111169);
  auto current_scene = Engine::SceneManager::GetCurrentScene();
  Engine::SceneManager::AddScene(_scene);
  Engine::SceneManager::OpenScene(_scene->GetID());

  auto window_size = glm::vec2(1600.0f, 900.0f);  // Engine::Window::Get().GetScreenSize();

  auto entity     = Engine::ECS::EntityManager::GetInstance().CreateEntity();
  auto uiRenderer = entity->AddComponent< Engine::Components::UIRenderer >();
  auto transform  = entity->AddComponent< Engine::Transform >();
  transform->Position(glm::vec3(window_size * 0.5f, 0.0f));

  _image = std::make_shared< Engine::Renderer::Image >();
  _image->Size(glm::vec2(window_size));
  stbi_set_flip_vertically_on_load(true);
  _image->Texture(Engine::AssetManager::GetTexture2D("./textures/UI/creditsy.png"));

  _exitButton = std::make_shared< Engine::Renderer::Button >();
  _exitButton->SelectedColor(glm::vec4(1.0f));
  _exitButton->Color(glm::vec4(0.6f, 0.6f, 0.6f, 1.0f));
  _exitButton->PressedColor(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
  _exitButton->OnPress([]() { GameManager::SwitchScene(SceneName::MainMenu); });
  _exitButton->text("");
  _exitButton->TextColor(glm::vec4(0, 0, 0, 1));
  _exitButton->TextOffset(glm::vec2(-35.0f, -15.0f));
  _exitButton->Size(glm::vec2(231.0f, 65.0f));
  _exitButton->HandleSize(glm::vec2(231.0f, 65.0f));
  _exitButton->Offset({0.0f, -400.0f});
  _exitButton->Background(Engine::AssetManager::GetTexture2D("./textures/UI/back.png"));

  uiRenderer->AddElement(_image);
  uiRenderer->AddButton(_exitButton);
  uiRenderer->AddElement(_exitButton);

  stbi_set_flip_vertically_on_load(false);

  auto gui = Engine::ECS::EntityManager::GetInstance().GetSystem< Engine::Systems::GUISystem >();
  gui->OnWindowResize(Engine::Window::Get().GetScreenSize());

  if (current_scene != nullptr) {
    Engine::SceneManager::OpenScene(current_scene->GetID());
  }
}

auto CreditsMenu::Scene() -> std::shared_ptr< Engine::Scene > {
  return _scene;
}
