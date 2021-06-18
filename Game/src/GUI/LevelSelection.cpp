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

  auto window_size = glm::vec2(1600.0f, 900.0f);  // Engine::Window::Get().GetScreenSize();
  stbi_set_flip_vertically_on_load(true);

  auto backgroundEntity     = Engine::ECS::EntityManager::GetInstance().CreateEntity();
  auto backgroundUIRenderer = backgroundEntity->AddComponent< Engine::Components::UIRenderer >();
  auto backgroundTransform  = backgroundEntity->AddComponent< Engine::Transform >();
  _background               = std::make_shared< Engine::Renderer::Image >();
  _background->Size(glm::vec2(window_size));
  _background->Texture(Engine::AssetManager::GetTexture2D("./textures/UI/tlo_skaly.png"));
  backgroundTransform->Position(glm::vec3(window_size * 0.5f, 0.0f));
  backgroundUIRenderer->AddElement(_background);

  _titleImage = std::make_shared< Engine::Renderer::Image >();
  _titleImage->Texture(
      Engine::AssetManager::GetTexture2D("./textures/UI/title_level_selection.png"));
  _titleImage->Size({419.0f, 74.0f});
  _titleImage->Offset({0.0f, 100.0f});
  backgroundUIRenderer->AddElement(_titleImage);

  auto entity     = Engine::ECS::EntityManager::GetInstance().CreateEntity();
  auto uiRenderer = entity->AddComponent< Engine::Components::UIRenderer >();
  auto transform  = entity->AddComponent< Engine::Transform >();
  _lvl1Button     = std::make_shared< Engine::Renderer::Button >();
  _lvl1Button->Background(Engine::AssetManager::GetTexture2D("./textures/UI/but_play.png"));
  _lvl1Button->Color(glm::vec4(0.6f, 0.6f, 0.6f, 1.0f));
  _lvl1Button->SelectedColor(glm::vec4(1.0f));
  _lvl1Button->PressedColor(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
  _lvl1Button->OnPress([]() { GameManager::SwitchScene(SceneName::Cutscene); });
  _lvl1Button->text("");
  _lvl1Button->TextColor(glm::vec4(0, 0, 0, 1));
  _lvl1Button->TextOffset(glm::vec2(-250.0f, -15.0f));
  _lvl1Button->Size(glm::vec2(66.0f, 64.0f));
  _lvl1Button->HandleSize(glm::vec2(66.0f, 64.0f));
  _lvl1Button->Offset({115.0f, 75.0f});
  transform->Position(glm::vec3(window_size.x * 0.5f, window_size.y * 0.5f - 100.0f, 0.0f));

  _lvl1Image = std::make_shared< Engine::Renderer::Image >();
  _lvl1Image->Texture(Engine::AssetManager::GetTexture2D("./textures/UI/lvl1.png"));
  _lvl1Image->Size({230.0f, 65.0f});
  _lvl1Image->Offset({-50.0f, 75.0f});

  _lvl2Button = std::make_shared< Engine::Renderer::Button >();
  _lvl2Button->Background(Engine::AssetManager::GetTexture2D("./textures/UI/but_play.png"));
  _lvl2Button->Color(glm::vec4(0.6f, 0.6f, 0.6f, 1.0f));
  _lvl2Button->SelectedColor(glm::vec4(1.0f));
  _lvl2Button->PressedColor(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
  _lvl2Button->OnPress([]() { GameManager::SwitchScene(SceneName::LVL_2); });
  _lvl2Button->text("");
  _lvl2Button->TextColor(glm::vec4(0, 0, 0, 1));
  _lvl2Button->TextOffset(glm::vec2(-250.0f, -15.0f));
  _lvl2Button->Size(glm::vec2(66.0f, 64.0f));
  _lvl2Button->HandleSize(glm::vec2(66.0f, 64.0f));
  _lvl2Button->Offset({115.0f, 5.0f});
  transform->Position(glm::vec3(window_size.x * 0.5f, window_size.y * 0.5f - 100.0f, 0.0f));

  _lvl2Image = std::make_shared< Engine::Renderer::Image >();
  _lvl2Image->Texture(Engine::AssetManager::GetTexture2D("./textures/UI/lvl2.png"));
  _lvl2Image->Size({230.0f, 65.0f});
  _lvl2Image->Offset({-50.0f, 5.0f});

  _buttonBackground = std::make_shared< Engine::Renderer::Image >();
  _buttonBackground->Texture(
      Engine::AssetManager::GetTexture2D("./textures/UI/white_background.png"));
  _buttonBackground->Size(glm::vec2(380.0f, 285.0f));
  _buttonBackground->Color(glm::vec4(1.0f, 1.0f, 1.0f, 0.6f));
  uiRenderer->AddElement(_buttonBackground);
  uiRenderer->AddButton(_lvl1Button);
  uiRenderer->AddElement(_lvl1Button);
  uiRenderer->AddElement(_lvl1Image);

  uiRenderer->AddButton(_lvl2Button);
  uiRenderer->AddElement(_lvl2Button);
  uiRenderer->AddElement(_lvl2Image);

  _returnButton = std::make_shared< Engine::Renderer::Button >();
  _returnButton->Background(Engine::AssetManager::GetTexture2D("./textures/UI/back.png"));
  _returnButton->SelectedColor(glm::vec4(1.0f));
  _returnButton->Color(glm::vec4(0.6f, 0.6f, 0.6f, 1.0f));
  _returnButton->PressedColor(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
  _returnButton->OnPress([]() { GameManager::SwitchScene(SceneName::MainMenu); });
  _returnButton->text("");
  _returnButton->TextColor(glm::vec4(0, 0, 0, 1));
  _returnButton->TextOffset(glm::vec2(-50.0f, -15.0f));
  _returnButton->Size(glm::vec2(230.0f, 65.0f));
  _returnButton->HandleSize(glm::vec2(230.0f, 65.0f));
  _returnButton->Offset({0.0f, -75.0f});
  uiRenderer->AddButton(_returnButton);
  uiRenderer->AddElement(_returnButton);

  stbi_set_flip_vertically_on_load(false);

  auto gui = Engine::ECS::EntityManager::GetInstance().GetSystem< Engine::Systems::GUISystem >();
  gui->OnWindowResize(Engine::Window::Get().GetScreenSize());

  if (current_scene != nullptr) {
    Engine::SceneManager::OpenScene(current_scene->GetID());
  }
}

auto LevelSelection::Scene() -> std::shared_ptr< Engine::Scene > {
  return _scene;
}
