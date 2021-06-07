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

  auto window_size = Engine::Window::Get().GetScreenSize();

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

  auto entity     = Engine::ECS::EntityManager::GetInstance().CreateEntity();
  auto uiRenderer = entity->AddComponent< Engine::Components::UIRenderer >();
  auto transform  = entity->AddComponent< Engine::Transform >();
  _exitButton     = std::make_shared< Engine::Renderer::Button >();
  uiRenderer->AddButton(_exitButton);
  uiRenderer->AddElement(_exitButton);
  _exitButton->Color(glm::vec4(1.0f));
  _exitButton->PressedColor(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
  _exitButton->OnPress([]() { std::exit(0); });
  _exitButton->text("Exit");
  _exitButton->TextColor(glm::vec4(0, 0, 0, 1));
  _exitButton->TextOffset(glm::vec2(-35.0f, -15.0f));
  _exitButton->Size(glm::vec2(300.0f, 100.0f));
  _exitButton->HandleSize(glm::vec2(300.0f, 100.0f));
  //transform->Position(glm::vec3(window_size.x * 0.5f, window_size.y * 0.5f - 150.0f, 0.0f));
  transform->Position(glm::vec3(window_size.x * 0.5f, window_size.y * 0.5f - 350.0f, 0.0f));

  auto entity2     = Engine::ECS::EntityManager::GetInstance().CreateEntity();
  auto uiRenderer2 = entity2->AddComponent< Engine::Components::UIRenderer >();
  auto transform2  = entity2->AddComponent< Engine::Transform >();
  _startButton     = std::make_shared< Engine::Renderer::Button >();
  //_startButton->Color(glm::vec4(1.0f, 1.0f, 1.0f, 0.5f));
  _startButton->Color(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
  _startButton->PressedColor(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
  _startButton->OnPress([]() { GameManager::SwitchScene(SceneName::LevelSelection); });
  _startButton->text("Start");
  _startButton->TextColor(glm::vec4(0, 0, 0, 1));
  _startButton->TextOffset(glm::vec2(-45.0f, -15.0f));
  _startButton->Size(glm::vec2(300.0f, 100.0f));
  _startButton->HandleSize(glm::vec2(300.0f, 100.0f));
  uiRenderer2->AddButton(_startButton);
  uiRenderer2->AddElement(_startButton);
  //transform2->Position(glm::vec3(window_size.x * 0.5f, window_size.y * 0.5f + 50.0f, 0.0f));
  transform2->Position(glm::vec3(window_size.x * 0.5f, window_size.y * 0.5f - 225.0f, 0.0f));

  if (current_scene != nullptr) {
    Engine::SceneManager::OpenScene(current_scene->GetID());
  }
}

auto MainMenu::Scene() -> std::shared_ptr< Engine::Scene > {
  return _scene;
}