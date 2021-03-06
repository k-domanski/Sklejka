#include "PauseMenu.h"

#include "GameManager.h"
#include "App/Window.h"
#include "Components/Transform.h"
#include "ECS/EntityManager.h"

PauseMenu::PauseMenu() {
  auto entity        = Engine::ECS::EntityManager::GetInstance().CreateEntity();
  _backroundRenderer = entity->AddComponent< Engine::Components::UIRenderer >();
  auto transform     = entity->AddComponent< Engine::Transform >();

  auto windowSize = glm::vec2(1600.0f, 900.0f);  // Engine::Window::Get().GetScreenSize();
  stbi_set_flip_vertically_on_load(true);

  _background = std::make_shared< Engine::Renderer::Image >();
  _background->Texture(Engine::AssetManager::GetTexture2D("./textures/UI/white_background.png"));
  _background->Size({380.0f, 350.0f});
  _background->Color(glm::vec4(1.0f, 1.0f, 1.0f, 0.6f));
  _background->Offset({0.0f, -50.0f});
  //_background->Color(glm::vec4(0.6f, 0.6f, 0.6f, 1.0f));
  transform->Position(glm::vec3(windowSize / 2.0f, 0.0f));

  _title = std::make_shared< Engine::Renderer::Image >();
  _title->Texture(Engine::AssetManager::GetTexture2D("./textures/UI/title_pause.png"));
  _title->Size({419.0f, 74.0f});
  _title->Offset({0.0f, 200.0f});
  _title->Color(glm::vec4(1.0f));

  auto buttonEntity    = Engine::ECS::EntityManager::GetInstance().CreateEntity();
  _buttonRenderer      = buttonEntity->AddComponent< Engine::Components::UIRenderer >();
  auto buttonTransform = buttonEntity->AddComponent< Engine::Transform >();
  _mainMenuButton      = std::make_shared< Engine::Renderer::Button >();
  _mainMenuButton->Background(Engine::AssetManager::GetTexture2D("./textures/UI/main_menu.png"));
  _mainMenuButton->Size(glm::vec2(230.0f, 65.0f));
  _mainMenuButton->HandleSize(glm::vec2(230.0f, 65.0f));
  _mainMenuButton->Offset({0.0f, -160.0f});
  _mainMenuButton->text("");
  _mainMenuButton->TextOffset(glm::vec2(-90.0f, -15.0f));
  _mainMenuButton->TextColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
  _mainMenuButton->OnPress([this]() {
    Engine::ECS::EntityManager::GetInstance()
        .GetSystem< Engine::Systems::GUISystem >()
        ->BreakChecking(true);
    this->Hide();
    GameManager::SwitchScene(SceneName::MainMenu);
  });
  _mainMenuButton->PressedColor(glm::vec4(0.6f, 0.6f, 0.6f, 1.0f));
  _mainMenuButton->Color(glm::vec4(0.6f, 0.6f, 0.6f, 1.0f));
  _mainMenuButton->SelectedColor(glm::vec4(1.0f));
  buttonTransform->Position(glm::vec3(windowSize / 2.0f, 0.0f));

  _resumeButton = std::make_shared< Engine::Renderer::Button >();
  _resumeButton->Background(Engine::AssetManager::GetTexture2D("./textures/UI/resume.png"));
  _resumeButton->Size(glm::vec2(230.0f, 65.0f));
  _resumeButton->HandleSize(glm::vec2(230.0f, 65.0f));
  _resumeButton->Offset({0.0f, 65.0f});
  _resumeButton->text("");
  _resumeButton->TextOffset(glm::vec2(-60.0f, -15.0f));
  _resumeButton->TextColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
  _resumeButton->OnPress([this]() { this->Hide(); });
  _resumeButton->PressedColor(glm::vec4(0.6f, 0.6f, 0.6f, 1.0f));
  _resumeButton->Color(glm::vec4(0.6f, 0.6f, 0.6f, 1.0f));
  _resumeButton->SelectedColor(glm::vec4(1.0f));

  _optionsButton = std::make_shared< Engine::Renderer::Button >();
  _optionsButton->Background(Engine::AssetManager::GetTexture2D("./textures/UI/options.png"));
  _optionsButton->Size(glm::vec2(230.0f, 65.0f));
  _optionsButton->HandleSize(glm::vec2(230.0f, 65.0f));
  _optionsButton->Offset({0.0f, -10.0f});
  _optionsButton->text("");
  _optionsButton->TextOffset(glm::vec2(-60.0f, -15.0f));
  _optionsButton->TextColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
  _optionsButton->OnPress([this]() {
    // this->Hide();
    // GameManager::SwitchScene(SceneName::Options);
    GameManager::ShowOptions([this]() { Show(false); });
    Hide(false);
  });
  _optionsButton->PressedColor(glm::vec4(0.6f, 0.6f, 0.6f, 1.0f));
  _optionsButton->Color(glm::vec4(0.6f, 0.6f, 0.6f, 1.0f));
  _optionsButton->SelectedColor(glm::vec4(1.0f));

  _tutorialButton = std::make_shared< Engine::Renderer::Button >();
  _tutorialButton->SelectedColor(glm::vec4(1.0f));
  _tutorialButton->Color(glm::vec4(0.6f, 0.6f, 0.6f, 1.0f));
  _tutorialButton->PressedColor(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
  _tutorialButton->OnPress([this]() {
    GameManager::ShowTutorial([this]() { Show(false); });
    Hide(false);
  });
  _tutorialButton->text("");
  _tutorialButton->TextColor(glm::vec4(0, 0, 0, 1));
  _tutorialButton->TextOffset(glm::vec2(-15.0f, -15.0f));
  _tutorialButton->Size(glm::vec2(231.0f, 65.0f));
  _tutorialButton->HandleSize(glm::vec2(231.0f, 65.0f));
  _tutorialButton->Offset(glm::vec2(0.0f, -85.0f));
  _tutorialButton->Background(Engine::AssetManager::GetTexture2D("./textures/UI/HOW_TO_PLAY.png"));

  _backroundRenderer->AddElement(_background);
  _backroundRenderer->AddElement(_title);

  _buttonRenderer->AddElement(_resumeButton);
  _buttonRenderer->AddButton(_resumeButton);

  _buttonRenderer->AddElement(_optionsButton);
  _buttonRenderer->AddButton(_optionsButton);

  _buttonRenderer->AddElement(_tutorialButton);
  _buttonRenderer->AddButton(_tutorialButton);

  _buttonRenderer->AddElement(_mainMenuButton);
  _buttonRenderer->AddButton(_mainMenuButton);

  _isVisible = true;
  stbi_set_flip_vertically_on_load(false);

  Hide();
}

auto PauseMenu::Show(bool pause) -> void {
  if (_isVisible)
    return;
  if (pause) {
    // GameManager::GetGameSettings()->GameTimeScale(0.0f);
    _playerTimeScaleBeforePausing = GameManager::GetGameSettings()->GameTimeScale();
    GameManager::GetGameSettings()->GameTimeScale(0.0f);
    GameManager::SetPaused(true);
  }

  _background->SetActive(true);
  _title->SetActive(true);
  _resumeButton->SetActive(true);
  _mainMenuButton->SetActive(true);
  _optionsButton->SetActive(true);
  _tutorialButton->SetActive(true);
  /*_backroundRenderer->AddElement(_background);
  _backroundRenderer->AddElement(_text);

  _resumeButtonRenderer->AddElement(_resumeButton);
  _resumeButtonRenderer->AddButton(_resumeButton);

  _mainMenuButtonRenderer->AddElement(_mainMenuButton);
  _mainMenuButtonRenderer->AddButton(_mainMenuButton);*/
  _isVisible = true;
  Engine::SceneManager::GetCurrentScene()->OnWindowResize(Engine::Window::Get().GetScreenSize());
}

auto PauseMenu::Hide(bool unpause) -> void {
  if (!_isVisible)
    return;
  if (unpause) {
    // GameManager::GetGameSettings()->GameTimeScale(1.0f);
    GameManager::GetGameSettings()->GameTimeScale(_playerTimeScaleBeforePausing);
    GameManager::SetPaused(false);
  }

  _background->SetActive(false);
  _title->SetActive(false);
  _resumeButton->SetActive(false);
  _mainMenuButton->SetActive(false);
  _optionsButton->SetActive(false);
  _tutorialButton->SetActive(false);
  /*_backroundRenderer->RemoveElement(_background);
  _backroundRenderer->RemoveElement(_text);

  _resumeButtonRenderer->RemoveElement(_resumeButton);
  _resumeButtonRenderer->RemoveButton(_resumeButton);

  _mainMenuButtonRenderer->RemoveElement(_mainMenuButton);
  _mainMenuButtonRenderer->RemoveButton(_mainMenuButton);*/
  _isVisible = false;
}

PauseMenu::~PauseMenu() {
  Engine::ECS::EntityManager::GetInstance().RemoveEntity(_backroundRenderer->GetEntity());
  Engine::ECS::EntityManager::GetInstance().RemoveEntity(_buttonRenderer->GetEntity());
}