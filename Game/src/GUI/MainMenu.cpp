#include "MainMenu.h"

#include "GameManager.h"
#include "Components/UIRenderer.h"
#include "Engine/SceneManager.h"
#include "ECS/EntityManager.h"

MainMenu::MainMenu() {
  _scene             = std::make_shared< Engine::Scene >(696969);
  auto current_scene = Engine::SceneManager::GetCurrentScene();
  Engine::SceneManager::AddScene(_scene);
  Engine::SceneManager::OpenScene(_scene->GetID());

  auto window_size = glm::vec2(1600.0f, 900.0f);  // Engine::Window::Get().GetScreenSize();

  auto entity3     = Engine::ECS::EntityManager::GetInstance().CreateEntity();
  auto uiRenderer3 = entity3->AddComponent< Engine::Components::UIRenderer >();
  auto transform3  = entity3->AddComponent< Engine::Transform >();
  _image           = std::make_shared< Engine::Renderer::Image >();
  _image->Size(glm::vec2(window_size));
  stbi_set_flip_vertically_on_load(true);
  _image->Texture(Engine::AssetManager::GetTexture2D("./textures/squirrelNinja.png"));
  stbi_set_flip_vertically_on_load(false);
  transform3->Position(glm::vec3(window_size * 0.5f, 0.0f));
  uiRenderer3->AddElement(_image);
  stbi_set_flip_vertically_on_load(true);
  auto entity     = Engine::ECS::EntityManager::GetInstance().CreateEntity();
  auto uiRenderer = entity->AddComponent< Engine::Components::UIRenderer >();
  auto transform  = entity->AddComponent< Engine::Transform >();
  _exitButton     = std::make_shared< Engine::Renderer::Button >();
  _exitButton->SelectedColor(glm::vec4(1.0f));
  _exitButton->Color(glm::vec4(0.7f, 0.7f, 0.7f, 1.0f));
  _exitButton->PressedColor(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
  _exitButton->OnPress([]() { std::exit(0); });
  _exitButton->text("");
  _exitButton->TextColor(glm::vec4(0, 0, 0, 1));
  _exitButton->TextOffset(glm::vec2(-35.0f, -15.0f));
  _exitButton->Size(glm::vec2(231.0f, 65.0f));
  _exitButton->HandleSize(glm::vec2(231.0f, 65.0f));
  // transform->Position(glm::vec3(window_size.x * 0.5f, window_size.y * 0.5f - 150.0f, 0.0f));
  transform->Position(glm::vec3(window_size.x * 0.5f, window_size.y * 0.5f - 325.0f, 0.0f));
  _exitButton->Background(Engine::AssetManager::GetTexture2D("./textures/UI/exit.png"));

  _startButton = std::make_shared< Engine::Renderer::Button >();
  //_startButton->Color(glm::vec4(1.0f, 1.0f, 1.0f, 0.5f));
  _startButton->SelectedColor(glm::vec4(1.0f));
  _startButton->Color(glm::vec4(0.7f, 0.7f, 0.7f, 1.0f));
  _startButton->PressedColor(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
  _startButton->OnPress([]() { GameManager::SwitchScene(SceneName::LevelSelection); });
  _startButton->text("");
  _startButton->TextColor(glm::vec4(0, 0, 0, 1));
  _startButton->TextOffset(glm::vec2(-45.0f, -15.0f));
  _startButton->Size(glm::vec2(231.0f, 65.0f));
  _startButton->HandleSize(glm::vec2(231.0f, 65.0f));
  // transform2->Position(glm::vec3(window_size.x * 0.5f, window_size.y * 0.5f + 50.0f, 0.0f));
  _startButton->Offset(glm::vec2(0.0f, 375.0f));
  _startButton->Background(Engine::AssetManager::GetTexture2D("./textures/UI/start.png"));

  _optionsButton = std::make_shared< Engine::Renderer::Button >();
  _optionsButton->SelectedColor(glm::vec4(1.0f));
  _optionsButton->Color(glm::vec4(0.7f, 0.7f, 0.7f, 1.0f));
  _optionsButton->PressedColor(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
  //_optionsButton->OnPress([this]() { GameManager::SwitchScene(SceneName::Options); });
  _optionsButton->OnPress([this]() {
    GameManager::ShowOptions([this]() { Show(); });
    Hide();
  });
  _optionsButton->text("");
  _optionsButton->TextColor(glm::vec4(0, 0, 0, 1));
  _optionsButton->TextOffset(glm::vec2(-45.0f, -15.0f));
  _optionsButton->Size(glm::vec2(231.0f, 65.0f));
  _optionsButton->HandleSize(glm::vec2(231.0f, 65.0f));
  _optionsButton->Offset(glm::vec2(0.0f, 250.0f));
  _optionsButton->Background(Engine::AssetManager::GetTexture2D("./textures/UI/options.png"));

  _creditsButton = std::make_shared< Engine::Renderer::Button >();
  _creditsButton->SelectedColor(glm::vec4(1.0f));
  _creditsButton->Color(glm::vec4(0.7f, 0.7f, 0.7f, 1.0f));
  _creditsButton->PressedColor(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
  _creditsButton->OnPress([]() { GameManager::SwitchScene(SceneName::LevelSelection); });
  _creditsButton->text("");
  _creditsButton->TextColor(glm::vec4(0, 0, 0, 1));
  _creditsButton->TextOffset(glm::vec2(-45.0f, -15.0f));
  _creditsButton->Size(glm::vec2(231.0f, 65.0f));
  _creditsButton->HandleSize(glm::vec2(231.0f, 65.0f));
  _creditsButton->Offset(glm::vec2(0.0f, 125.0f));
  _creditsButton->Background(Engine::AssetManager::GetTexture2D("./textures/UI/credits.png"));
  stbi_set_flip_vertically_on_load(false);
  uiRenderer->AddButton(_startButton);
  uiRenderer->AddElement(_startButton);
  uiRenderer->AddButton(_optionsButton);
  uiRenderer->AddElement(_optionsButton);
  uiRenderer->AddButton(_creditsButton);
  uiRenderer->AddElement(_creditsButton);
  uiRenderer->AddButton(_exitButton);
  uiRenderer->AddElement(_exitButton);

  auto gui = Engine::ECS::EntityManager::GetInstance().GetSystem< Engine::Systems::GUISystem >();
  gui->OnWindowResize(Engine::Window::Get().GetScreenSize());

  if (current_scene != nullptr) {
    Engine::SceneManager::OpenScene(current_scene->GetID());
  }
}

auto MainMenu::Scene() -> std::shared_ptr< Engine::Scene > {
  return _scene;
}

auto MainMenu::Show() -> void {
  _exitButton->SetActive(true);
  _optionsButton->SetActive(true);
  _creditsButton->SetActive(true);
  _startButton->SetActive(true);
}

auto MainMenu::Hide() -> void {
  _exitButton->SetActive(false);
  _optionsButton->SetActive(false);
  _creditsButton->SetActive(false);
  _startButton->SetActive(false);
}
