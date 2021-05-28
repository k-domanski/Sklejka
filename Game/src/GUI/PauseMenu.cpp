#include "PauseMenu.h"

#include "GameManager.h"
#include "App/Window.h"
#include "Components/Transform.h"
#include "ECS/EntityManager.h"

PauseMenu::PauseMenu() {
  auto entity        = Engine::ECS::EntityManager::GetInstance().CreateEntity();
  _backroundRenderer = entity->AddComponent< Engine::Components::UIRenderer >();
  auto transform     = entity->AddComponent< Engine::Transform >();

  auto windowSize = Engine::Window::Get().GetScreenSize();

  _background = std::make_shared< Engine::Renderer::Image >();
  _background->Size(windowSize / 2.0f);
  _background->Color(glm::vec4(0.6f, 0.6f, 0.6f, 1.0f));
  transform->Position(glm::vec3(windowSize / 2.0f, 0.0f));

  _text = std::make_shared< Engine::Renderer::Text >();
  _text->SetText("Paused");
  _text->Color(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
  _text->Offset(glm::vec2(-75.0f, 100.0f));
  _text->Size(1);

  auto entity1            = Engine::ECS::EntityManager::GetInstance().CreateEntity();
  _mainMenuButtonRenderer = entity1->AddComponent< Engine::Components::UIRenderer >();
  auto transform1         = entity1->AddComponent< Engine::Transform >();
  _mainMenuButton         = std::make_shared< Engine::Renderer::Button >();
  _mainMenuButton->Size(glm::vec2(300.0f, 100.0f));
  _mainMenuButton->HandleSize(glm::vec2(300.0f, 100.0f));
  _mainMenuButton->text("Main menu");
  _mainMenuButton->TextOffset(glm::vec2(-75.0f, -20.0f));
  _mainMenuButton->TextColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
  _mainMenuButton->OnPress([this]() {
    Engine::ECS::EntityManager::GetInstance()
        .GetSystem< Engine::Systems::GUISystem >()
        ->BreakChecking(true);
    this->Hide();
    GameManager::SwitchScene(SceneName::MainMenu);
  });
  _mainMenuButton->PressedColor(glm::vec4(0.6f, 0.6f, 0.6f, 1.0f));
  _mainMenuButton->Color(glm::vec4(1.0f));
  transform1->Position(glm::vec3(windowSize.x / 2.0f, windowSize.y / 2.0f - 150.0f, 0.0f));

  auto entity2          = Engine::ECS::EntityManager::GetInstance().CreateEntity();
  _resumeButtonRenderer = entity2->AddComponent< Engine::Components::UIRenderer >();
  auto transform2       = entity2->AddComponent< Engine::Transform >();
  _resumeButton         = std::make_shared< Engine::Renderer::Button >();
  _resumeButton->Size(glm::vec2(300.0f, 100.0f));
  _resumeButton->HandleSize(glm::vec2(300.0f, 100.0f));
  _resumeButton->text("Resume");
  _resumeButton->TextOffset(glm::vec2(-50.0f, -20.0f));
  _resumeButton->TextColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
  _resumeButton->OnPress([this]() { this->Hide(); });
  _resumeButton->PressedColor(glm::vec4(0.6f, 0.6f, 0.6f, 1.0f));
  _resumeButton->Color(glm::vec4(1.0f));
  transform2->Position(glm::vec3(windowSize.x / 2.0f, windowSize.y / 2.0f, 0.0f));

  _backroundRenderer->AddElement(_background);
  _backroundRenderer->AddElement(_text);

  _resumeButtonRenderer->AddElement(_resumeButton);
  _resumeButtonRenderer->AddButton(_resumeButton);

  _mainMenuButtonRenderer->AddElement(_mainMenuButton);
  _mainMenuButtonRenderer->AddButton(_mainMenuButton);
  _isVisible = true;
  Hide();
}

auto PauseMenu::Show() -> void {
  if (_isVisible)
    return;
  // GameManager::GetGameSettings()->GameTimeScale(0.0f);
  GameManager::GetGameSettings()->PlayerTimeScale(0.0f);

  _background->SetActive(true);
  _text->SetActive(true);
  _resumeButton->SetActive(true);
  _mainMenuButton->SetActive(true);
  /*_backroundRenderer->AddElement(_background);
  _backroundRenderer->AddElement(_text);

  _resumeButtonRenderer->AddElement(_resumeButton);
  _resumeButtonRenderer->AddButton(_resumeButton);

  _mainMenuButtonRenderer->AddElement(_mainMenuButton);
  _mainMenuButtonRenderer->AddButton(_mainMenuButton);*/
  _isVisible = true;
}

auto PauseMenu::Hide() -> void {
  if (!_isVisible)
    return;
  // GameManager::GetGameSettings()->GameTimeScale(1.0f);
  GameManager::GetGameSettings()->PlayerTimeScale(1.0f);

  _background->SetActive(false);
  _text->SetActive(false);
  _resumeButton->SetActive(false);
  _mainMenuButton->SetActive(false);
  /*_backroundRenderer->RemoveElement(_background);
  _backroundRenderer->RemoveElement(_text);

  _resumeButtonRenderer->RemoveElement(_resumeButton);
  _resumeButtonRenderer->RemoveButton(_resumeButton);

  _mainMenuButtonRenderer->RemoveElement(_mainMenuButton);
  _mainMenuButtonRenderer->RemoveButton(_mainMenuButton);*/
  _isVisible = false;
}

PauseMenu::~PauseMenu() {
  Engine::ECS::EntityManager::GetInstance().RemoveEntity(_backroundRenderer->GetEntityID());
  Engine::ECS::EntityManager::GetInstance().RemoveEntity(_resumeButtonRenderer->GetEntityID());
  Engine::ECS::EntityManager::GetInstance().RemoveEntity(_mainMenuButtonRenderer->GetEntityID());
}