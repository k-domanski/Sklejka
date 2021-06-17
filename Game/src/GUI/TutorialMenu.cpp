#include "TutorialMenu.h"

#include "GameManager.h"
#include "App/AssetManager.h"
#include "App/Window.h"
#include "Engine/SceneManager.h"
#include "Systems/GUISystem.h"

TutorialMenu::TutorialMenu() {
  _scene             = std::make_shared< Engine::Scene >(6977777697777769);
  auto current_scene = Engine::SceneManager::GetCurrentScene();
  Engine::SceneManager::AddScene(_scene);
  Engine::SceneManager::OpenScene(_scene->GetID());

  _isVisible = false;

  auto window_size = glm::vec2(1600.0f, 900.0f);  // Engine::Window::Get().GetScreenSize();

  _background = std::make_shared< Engine::Renderer::Image >();
  _background->Size(glm::vec2(window_size));
  stbi_set_flip_vertically_on_load(true);
  _background->Texture(Engine::AssetManager::GetTexture2D("./textures/UI/tutorial.png"));


  _returnButton = std::make_shared< Engine::Renderer::Button >();
  _returnButton->SelectedColor(glm::vec4(1.0f));
  _returnButton->Color(glm::vec4(0.6f, 0.6f, 0.6f, 1.0f));
  _returnButton->PressedColor(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
  _returnButton->OnPress([]() { GameManager::SwitchScene(SceneName::MainMenu); });
  _returnButton->text("");
  _returnButton->TextColor(glm::vec4(0, 0, 0, 1));
  _returnButton->TextOffset(glm::vec2(-35.0f, -15.0f));
  _returnButton->Size(glm::vec2(231.0f, 65.0f));
  _returnButton->HandleSize(glm::vec2(231.0f, 65.0f));
  _returnButton->Offset({0.0f, -400.0f});
  _returnButton->Background(Engine::AssetManager::GetTexture2D("./textures/UI/back.png"));

  stbi_set_flip_vertically_on_load(false);

  auto gui = Engine::ECS::EntityManager::GetInstance().GetSystem< Engine::Systems::GUISystem >();
  gui->OnWindowResize(Engine::Window::Get().GetScreenSize());

  if (current_scene != nullptr) {
    Engine::SceneManager::OpenScene(current_scene->GetID());
  }
}

auto TutorialMenu::Scene() -> std::shared_ptr< Engine::Scene > {
  return _scene;
}

auto TutorialMenu::IsVisible() -> bool {
  return _isVisible;
}

auto TutorialMenu::Show(std::function< void() > returnFunc) -> void {
  if (_isVisible)
    return;
  AddAllElements();

  _returnButton->OnPress([this, returnFunc]() {
    returnFunc();
    Hide();
  });

  _isVisible = true;
}

auto TutorialMenu::Hide() -> void {
  if (!_isVisible)
    return;
  RemoveAllElements();

  _isVisible = false;
}

auto TutorialMenu::HideFromButton() -> void {
  _returnButton->TriggerOnPress();
}

auto TutorialMenu::AddAllElements() -> void {
  auto window_size   = Engine::Window::Get().GetScreenSize();  // glm::vec2(1600.0f, 900.0f);
  _entity    = Engine::ECS::EntityManager::GetInstance().CreateEntity();
  auto uiRenderer    = _entity->AddComponent< Engine::Components::UIRenderer >();
  auto transform = _entity->AddComponent< Engine::Transform >();
  transform->Position(glm::vec3(window_size * 0.5f, 0.0f));

  uiRenderer->AddElement(_background);
  uiRenderer->AddElement(_returnButton);
  uiRenderer->AddButton(_returnButton);

}

auto TutorialMenu::RemoveAllElements() -> void {
  Engine::ECS::EntityManager::GetInstance().RemoveEntity(_entity);
}
