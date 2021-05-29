#include "EndLevelMenu.h"
#include "GameManager.h"
#include "App/Window.h"
#include "Components/Transform.h"
#include "ECS/EntityManager.h"

EndLevelMenu::EndLevelMenu() {
  auto entity = Engine::ECS::EntityManager ::GetInstance().CreateEntity();
  _imageRenderer = entity->AddComponent< Engine::Components::UIRenderer >();
  auto transform  = entity->AddComponent< Engine::Transform >();
  _image         = std::make_shared< Engine::Renderer::Image >();
  auto windowSize = Engine::Window::Get().GetScreenSize();
  _image->Size(windowSize / 2.0f);
  transform->Position(glm::vec3(windowSize / 2.0f, 0.0f));
  _image->Color(glm::vec4(0.6f, 0.6f, 0.6f, 1.0f));
  _timeText = std::make_shared< Engine::Renderer::Text >();
  _timeText->SetText("");
  _timeText->Size(1);
  _timeText->Color(glm::vec4(1.0f));
  _timeText->Offset(glm::vec2(0.0f, 100.0f));

  auto entity1    = Engine::ECS::EntityManager ::GetInstance().CreateEntity();
  _tryAgainRenderer = entity1->AddComponent< Engine::Components::UIRenderer >();
  auto transform1 = entity1->AddComponent< Engine::Transform >();
  _tryAgainButton   = std::make_shared< Engine::Renderer::Button >();
  _tryAgainButton->Size(glm::vec2(300.0f, 100.0));
  _tryAgainButton->HandleSize(glm::vec2(300.0f, 100.0));
  _tryAgainButton->OnPress([]() { GameManager::SwitchScene(SceneName::LVL_1); });//TODO: change for level reload
  _tryAgainButton->text("Try Again");
  _tryAgainButton->TextColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
  _tryAgainButton->TextOffset(glm::vec2(-50.0f, -20.0f));
  transform1->Position(glm::vec3(windowSize.x / 2.0f, windowSize.y / 2.0f ,0.0f));

  auto entity2    = Engine::ECS::EntityManager ::GetInstance().CreateEntity();
  _nextLevelRenderer = entity2->AddComponent< Engine::Components::UIRenderer >();
  auto transform2 = entity2->AddComponent< Engine::Transform >();
  _nextLevelButton   = std::make_shared< Engine::Renderer::Button >();
  _nextLevelButton->Size(glm::vec2(300.0f, 100.0));
  _nextLevelButton->HandleSize(glm::vec2(300.0f, 100.0));
  _nextLevelButton->OnPress([]() { GameManager::SwitchScene(SceneName::LVL_1); });//TODO: change for next level load
  _nextLevelButton->text("Next level");
  _nextLevelButton->TextColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
  _nextLevelButton->TextOffset(glm::vec2(-50.0f, -20.0f));
  transform2->Position(glm::vec3(windowSize.x / 2.0f, windowSize.y / 2.0f - 100.0f ,0.0f));
}

auto EndLevelMenu::Show(std::string text) -> void {
  _imageRenderer->AddElement(_image);
  _imageRenderer->AddElement(_timeText);
  _timeText->SetText(text);

  _tryAgainRenderer->AddElement(_tryAgainButton);
  _tryAgainRenderer->AddButton(_tryAgainButton);

  _nextLevelRenderer->AddButton(_nextLevelButton);
  _nextLevelRenderer->AddElement(_nextLevelButton);
}

auto EndLevelMenu::Hide() -> void {
  _imageRenderer->RemoveElement(_image);
  _imageRenderer->RemoveElement(_timeText);
  _timeText->SetText("");

  _tryAgainRenderer->RemoveElement(_tryAgainButton);
  _tryAgainRenderer->RemoveButton(_tryAgainButton);

  _nextLevelRenderer->RemoveButton(_nextLevelButton);
  _nextLevelRenderer->RemoveElement(_nextLevelButton);
}