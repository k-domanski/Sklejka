#include "LevelSelection.h"

#include "GameManager.h"
#include "App/AssetManager.h"
#include "App/Window.h"
#include "Components/Transform.h"
#include "Components/UIRenderer.h"
#include "Engine/SceneManager.h"
#include "Engine/Scene.h"
#include "ECS/EntityManager.h"

LevelSelection::LevelSelection() {
  _scene             = std::make_shared< Engine::Scene >(969696);
  auto current_scene = Engine::SceneManager::GetCurrentScene();
  Engine::SceneManager::AddScene(_scene);
  Engine::SceneManager::OpenScene(_scene->GetID());

  auto window_size = Engine::Window::Get().GetScreenSize();

  auto backgroundEntity     = Engine::ECS::EntityManager::GetInstance().CreateEntity();
  auto backgroundUIRenderer = backgroundEntity->AddComponent< Engine::Components::UIRenderer >();
  auto backgroundTransform  = backgroundEntity->AddComponent< Engine::Transform >();
  _background               = std::make_shared< Engine::Renderer::Image >();
  _background->Size(glm::vec2(window_size));
  _background->Texture(Engine::AssetManager::GetTexture2D("./textures/squirrelNinja.png"));
  backgroundTransform->Position(glm::vec3(window_size * 0.5f, 0.0f));
  backgroundUIRenderer->AddElement(_background);

  auto entity     = Engine::ECS::EntityManager::GetInstance().CreateEntity();
  auto uiRenderer = entity->AddComponent< Engine::Components::UIRenderer >();
  auto transform  = entity->AddComponent< Engine::Transform >();
  _lvl1Button     = std::make_shared< Engine::Renderer::Button >();
  _lvl1Button->Color(glm::vec4(0.8f, 0.8f, 0.8f, 1.0f));
  _lvl1Button->SelectedColor(glm::vec4(1.0f));
  _lvl1Button->PressedColor(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
  _lvl1Button->OnPress([]() { GameManager::SwitchScene(SceneName::Cutscene); });
  _lvl1Button->text("Level 1");
  _lvl1Button->TextColor(glm::vec4(0, 0, 0, 1));
  _lvl1Button->TextOffset(glm::vec2(-250.0f, -15.0f));
  _lvl1Button->Size(glm::vec2(100.0f, 100.0f));
  _lvl1Button->HandleSize(glm::vec2(100.0f, 100.0f));
  _lvl1Button->Offset({100.0f, 0.0f});
  stbi_set_flip_vertically_on_load(true);
  _lvl1Button->Background(Engine::AssetManager::GetTexture2D("./textures/arrow_green.png"));
  stbi_set_flip_vertically_on_load(false);
  transform->Position(glm::vec3(window_size.x * 0.5f, window_size.y * 0.5f - 100.0f, 0.0f));

  _buttonBackground = std::make_shared< Engine::Renderer::Image >();
  _buttonBackground->Size(glm::vec2(500.0f, 600.0f));
  _buttonBackground->Color(glm::vec4(1.0f, 1.0f, 1.0f, 0.8f));
  uiRenderer->AddElement(_buttonBackground);
  uiRenderer->AddButton(_lvl1Button);
  uiRenderer->AddElement(_lvl1Button);

  _returnButton     = std::make_shared< Engine::Renderer::Button >();
  _returnButton->SelectedColor(glm::vec4(1.0f));
  _returnButton->Color({0.8f, 0.8f, 0.8f, 1.0f});
  _returnButton->PressedColor(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
  _returnButton->OnPress([]() { GameManager::SwitchScene(SceneName::MainMenu); });
  _returnButton->text("Return");
  _returnButton->TextColor(glm::vec4(0, 0, 0, 1));
  _returnButton->TextOffset(glm::vec2(-50.0f, -15.0f));
  _returnButton->Size(glm::vec2(200.0f, 75.0f));
  _returnButton->HandleSize(glm::vec2(200.0f, 75.0f));
  _returnButton->Offset({0.0f, -250.0f});
  uiRenderer->AddButton(_returnButton);
  uiRenderer->AddElement(_returnButton);

  _title = std::make_shared< Engine::Renderer::Text >();
  _title->Size(2);
  _title->SetText("Level select");
  _title->Color(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
  _title->Offset(glm::vec2(-200.0f,  -20.0f));

  auto titleEntity     = Engine::ECS::EntityManager::GetInstance().CreateEntity();
  auto titleUIRenderer = titleEntity->AddComponent< Engine::Components::UIRenderer >();
  auto titleTransform  = titleEntity->AddComponent< Engine::Transform >();
  _titleBackground = std::make_shared< Engine::Renderer::Image >();
  _titleBackground->Size(glm::vec2(500.0f, 150.0f));
  _titleBackground->Color(glm::vec4(1.0f, 1.0f, 1.0f, 0.8f));

  titleTransform->Position(glm::vec3(window_size.x * 0.5f, window_size.y * 0.5f + 300.0f, 0.0f));
  titleUIRenderer->AddElement(_titleBackground);
  titleUIRenderer->AddElement(_title);
  

  if (current_scene != nullptr) {
    Engine::SceneManager::OpenScene(current_scene->GetID());
  }
}

auto LevelSelection::Scene() -> std::shared_ptr< Engine::Scene > {
  return _scene;
}
