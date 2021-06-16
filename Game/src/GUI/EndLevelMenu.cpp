#include "EndLevelMenu.h"

#include <iomanip>

#include "GameManager.h"
#include "App/Window.h"
#include "Components/Transform.h"
#include "ECS/EntityManager.h"

EndLevelMenu::EndLevelMenu() {
  _visible = false;

  auto entity    = Engine::ECS::EntityManager ::GetInstance().CreateEntity();
  _imageRenderer = entity->AddComponent< Engine::Components::UIRenderer >();
  auto transform = entity->AddComponent< Engine::Transform >();

  auto windowSize = glm::vec2(1600.0f, 900.0f);  // Engine::Window::Get().GetScreenSize();

  stbi_set_flip_vertically_on_load(true);

  _background = std::make_shared< Engine::Renderer::Image >();
  _background->Texture(Engine::AssetManager::GetTexture2D("./textures/UI/white_background.png"));
  _background->Size({520.0f, 320.0f});
  _background->Color(glm::vec4(1.0f, 1.0f, 1.0f, 0.6f));
  _background->Offset({0.0f, -10.0f});
  transform->Position(glm::vec3(windowSize / 2.0f, 0.0f));

  _title = std::make_shared< Engine::Renderer::Image >();
  _title->Texture(Engine::AssetManager::GetTexture2D("./textures/UI/title_level_completed.png"));
  _title->Texture(Engine::AssetManager::GetTexture2D("./textures/UI/title_crashed.png"));
  _title->Size({419.0f, 75.0f});
  _title->Offset({0.0f, 200.0f});

  _timeText = std::make_shared< Engine::Renderer::Text >();
  _timeText->SetText("");
  _timeText->Size(1);
  _timeText->Color(glm::vec4(1.0f));
  _timeText->Offset(glm::vec2(-150.0f, 100.0f));

  _bellText = std::make_shared< Engine::Renderer::Text >();
  _bellText->SetText("");
  _bellText->Size(1);
  _bellText->Color(glm::vec4(1.0f));
  _bellText->Offset(glm::vec2(-150.0f, 100.0f));

  auto entity1    = Engine::ECS::EntityManager ::GetInstance().CreateEntity();
  _buttonRenderer = entity1->AddComponent< Engine::Components::UIRenderer >();
  auto transform1 = entity1->AddComponent< Engine::Transform >();

  _tryAgainButton = std::make_shared< Engine::Renderer::Button >();
  _tryAgainButton->Background(
      Engine::AssetManager::GetTexture2D("./textures/UI/try_again_small.png"));
  _tryAgainButton->Background(Engine::AssetManager::GetTexture2D("./textures/UI/try_again.png"));
  _tryAgainButton->Size(glm::vec2(230.0f, 65.0));
  _tryAgainButton->HandleSize(glm::vec2(230.0f, 65.0f));
  _tryAgainButton->Color(glm::vec4(0.6f, 0.6f, 0.6f, 1.0f));
  _tryAgainButton->SelectedColor(glm::vec4(1.0f));
  _tryAgainButton->PressedColor({0.5f, 0.5f, 0.5f, 1.0f});
  _tryAgainButton->OnPress([]() { GameManager::SwitchScene(GameManager::GetCurrentSceneName()); });
  _tryAgainButton->text("");
  _tryAgainButton->TextColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
  _tryAgainButton->TextOffset(glm::vec2(-70.0f, -15.0f));
  transform1->Position(glm::vec3(windowSize.x / 2.0f, windowSize.y / 2.0f, 0.0f));

  _nextLevelButton = std::make_shared< Engine::Renderer::Button >();
  _nextLevelButton->Background(Engine::AssetManager::GetTexture2D("./textures/UI/next.png"));
  _nextLevelButton->Size(glm::vec2(165.0f, 65.0));
  _nextLevelButton->HandleSize(glm::vec2(165.0f, 65.0));
  _nextLevelButton->Color(glm::vec4(0.6f, 0.6f, 0.6f, 1.0f));
  _nextLevelButton->SelectedColor(glm::vec4(1.0f));
  _nextLevelButton->PressedColor({0.5f, 0.5f, 0.5f, 1.0f});
  auto nextScene = GameManager::GetNextSceneName();
  if (nextScene == +SceneName::MainMenu)
    nextScene = SceneName::LevelSelection;
  _nextLevelButton->OnPress([nextScene]() { GameManager::SwitchScene(nextScene); });
  _nextLevelButton->text("");
  _nextLevelButton->TextColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
  _nextLevelButton->TextOffset(glm::vec2(-50.0f, -20.0f));

  _mainMenuButton = std::make_shared< Engine::Renderer::Button >();
  _mainMenuButton->Background(Engine::AssetManager::GetTexture2D("./textures/UI/menu.png"));
  _mainMenuButton->Background(Engine::AssetManager::GetTexture2D("./textures/UI/main_menu.png"));
  _mainMenuButton->Size(glm::vec2(230.0f, 65.0));
  _mainMenuButton->HandleSize(glm::vec2(230.0f, 65.0f));
  _mainMenuButton->Color(glm::vec4(0.6f, 0.6f, 0.6f, 1.0f));
  _mainMenuButton->SelectedColor(glm::vec4(1.0f));
  _mainMenuButton->PressedColor({0.5f, 0.5f, 0.5f, 1.0f});
  _mainMenuButton->OnPress([this]() { GameManager::SwitchScene(SceneName::MainMenu); });
  _mainMenuButton->text("");
  _mainMenuButton->TextColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
  _mainMenuButton->TextOffset(glm::vec2(-70.0f, -15.0f));

  _deathMark = std::make_shared< Engine::Renderer::Image >();
  _deathMark->Texture(Engine::AssetManager::GetTexture2D("./textures/UI/skull.png"));
  _deathMark->Size({172.0f, 156.0f});
  _deathMark->Offset({0.0f, 60.0f});

  stbi_set_flip_vertically_on_load(false);

  //_imageRenderer->AddElement(_background);
  //_imageRenderer->AddElement(_title);
  //_imageRenderer->AddElement(_timeText);
  //_imageRenderer->AddElement(_deathMark);

  //_buttonRenderer->AddElement(_tryAgainButton);
  //_buttonRenderer->AddButton(_tryAgainButton);
  //_buttonRenderer->AddElement(_mainMenuButton);
  //_buttonRenderer->AddButton(_mainMenuButton);
  //_buttonRenderer->AddButton(_nextLevelButton);
  //_buttonRenderer->AddElement(_nextLevelButton);
  // Hide();
}

auto EndLevelMenu::Show(bool win, float time, int bells) -> void {
  if (_visible)
    return;
  if (win) {
    _imageRenderer->AddElement(_background);
    _title->Texture(Engine::AssetManager::GetTexture2D("./textures/UI/title_level_completed.png"));
    _title->Size({453.0f, 75.0f});
    _imageRenderer->AddElement(_title);
    std::stringstream s;
    s << "TIME:     " << std::fixed << std::setprecision(2) << time;
    _timeText->SetText(s.str());
    _timeText->Offset({-150.0f, 70.0f});
    _imageRenderer->AddElement(_timeText);
    _bellText->SetText("BELLS:     " + std::to_string(bells));
    _bellText->Offset({-175.0f, 0.0f});
    _imageRenderer->AddElement(_bellText);

    _mainMenuButton->Background(Engine::AssetManager::GetTexture2D("./textures/UI/menu.png"));
    _mainMenuButton->Size({165.0f, 65.0f});
    _mainMenuButton->HandleSize({165.0f, 65.0f});
    _mainMenuButton->Offset({-170.0f, -80.0f});
    _buttonRenderer->AddElement(_mainMenuButton);
    _buttonRenderer->AddButton(_mainMenuButton);

    _tryAgainButton->Background(
        Engine::AssetManager::GetTexture2D("./textures/UI/try_again_small.png"));
    _tryAgainButton->Size({165.0f, 65.0f});
    _tryAgainButton->HandleSize({165.0f, 65.0f});
    _tryAgainButton->Offset({0.0f, -80.0f});
    _buttonRenderer->AddElement(_tryAgainButton);
    _buttonRenderer->AddButton(_tryAgainButton);

    _nextLevelButton->Background(Engine::AssetManager::GetTexture2D("./textures/UI/next.png"));
    _nextLevelButton->Size({165.0f, 65.0f});
    _nextLevelButton->HandleSize({165.0f, 65.0f});
    _nextLevelButton->Offset({170.0f, -80.0f});
    _buttonRenderer->AddElement(_nextLevelButton);
    _buttonRenderer->AddButton(_nextLevelButton);
  } else {
    _imageRenderer->AddElement(_background);
    _title->Texture(Engine::AssetManager::GetTexture2D("./textures/UI/title_crashed.png"));
    _title->Size({419.0f, 75.0f});
    _imageRenderer->AddElement(_title);
    _imageRenderer->AddElement(_deathMark);

    _tryAgainButton->Background(Engine::AssetManager::GetTexture2D("./textures/UI/try_again.png"));
    _tryAgainButton->Size({230.0f, 65.0f});
    _tryAgainButton->HandleSize({230.0f, 65.0f});
    _tryAgainButton->Offset({0.0f, -60.0f});
    _buttonRenderer->AddElement(_tryAgainButton);
    _buttonRenderer->AddButton(_tryAgainButton);

    _mainMenuButton->Background(Engine::AssetManager::GetTexture2D("./textures/UI/main_menu.png"));
    _mainMenuButton->Size({230.0f, 65.0f});
    _mainMenuButton->HandleSize({230.0f, 65.0f});
    _mainMenuButton->Offset({0.0f, -130.0f});
    _buttonRenderer->AddElement(_mainMenuButton);
    _buttonRenderer->AddButton(_mainMenuButton);
  }

  _visible = true;
  _win     = win;
  auto gui = Engine::ECS::EntityManager::GetInstance().GetSystem< Engine::Systems::GUISystem >();
  gui->OnWindowResize(Engine::Window::Get().GetScreenSize());
}

auto EndLevelMenu::Hide() -> void {
  if (!_visible)
    return;
  if (_win) {
    _imageRenderer->RemoveElement(_background);
    _imageRenderer->RemoveElement(_title);
    _imageRenderer->RemoveElement(_timeText);
    _imageRenderer->RemoveElement(_bellText);

    _buttonRenderer->RemoveElement(_tryAgainButton);
    _buttonRenderer->RemoveButton(_tryAgainButton);
    _buttonRenderer->RemoveElement(_mainMenuButton);
    _buttonRenderer->RemoveButton(_mainMenuButton);
    _buttonRenderer->RemoveElement(_nextLevelButton);
    _buttonRenderer->RemoveButton(_nextLevelButton);
  } else {
    _imageRenderer->RemoveElement(_background);
    _imageRenderer->RemoveElement(_title);
    _imageRenderer->RemoveElement(_deathMark);

    _buttonRenderer->RemoveElement(_tryAgainButton);
    _buttonRenderer->RemoveButton(_tryAgainButton);
    _buttonRenderer->RemoveElement(_mainMenuButton);
    _buttonRenderer->RemoveButton(_mainMenuButton);
  }

  _visible = false;
}