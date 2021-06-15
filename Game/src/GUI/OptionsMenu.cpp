#include "OptionsMenu.h"

//#include "GameManager.h"
#include "GameManager.h"
#include "App/AssetManager.h"
#include "App/Window.h"
#include "Components/Transform.h"
#include "Components/UIRenderer.h"
#include "Engine/SceneManager.h"
#include "Engine/Scene.h"
#include "ECS/EntityManager.h"
#include "Renderer/Bar.h"

size_t OptionsMenu::OpenedFromScene() const {
  return _openedFromScene;
}

void OptionsMenu::OpenedFromScene(size_t opened_from_scene, int name) {
  _openedFromScene     = opened_from_scene;
  _openedFromSceneName = name;
}

OptionsMenu::OptionsMenu() {
  _isVisible           = false;
  _openedFromScene     = 696969;
  _openedFromSceneName = SceneName::MainMenu;
  _scene               = std::make_shared< Engine::Scene >(777777);
  auto current_scene   = Engine::SceneManager::GetCurrentScene();
  Engine::SceneManager::AddScene(_scene);
  Engine::SceneManager::OpenScene(_scene->GetID());

  auto window_size =
      glm::vec2(1600.0f, 900.0f);  // reference size  // Engine::Window::Get().GetScreenSize();

  stbi_set_flip_vertically_on_load(true);

  _background = std::make_shared< Engine::Renderer::Image >();
  _background->Texture(Engine::AssetManager::GetTexture2D("./textures/UI/white_background.png"));
  _background->Size({500.0f, 600.0f});
  _background->Color({1.0f, 1.0f, 1.0f, 0.6f});
  _background->Offset({0.0f, -22.0f});
  _title = std::make_shared< Engine::Renderer::Text >();
  _title->Size(1);
  _title->SetText("");
  _title->Offset(glm::vec2(0.0f, 300.0f));

  _titleImage = std::make_shared< Engine::Renderer::Image >();
  _titleImage->Texture(Engine::AssetManager::GetTexture2D("./textures/UI/title_options.png"));
  _titleImage->Size({419.0f, 74.0f});

  _musicData.currentValue = 10.0f;
  _musicData.maxValue     = 100.0f;
  _musicData.minValue     = 0.0f;
  _musicData.bar          = std::make_shared< Engine::Renderer::Bar >();
  _musicData.bar->BackgroundTexture(Engine::AssetManager::GetTexture2D("./textures/UI/music.png"));
  _musicData.bar->FillTexture(Engine::AssetManager::GetTexture2D("./textures/UI/slider.png"));
  _musicData.bar->Size(glm::vec2(298.0f, 74.0f));
  _musicData.bar->FillAreaSize(glm::vec2(274.0f, 19.0f));
  _musicData.bar->FillAreaOffset(glm::vec2(0.0f, -20.0f));
  _musicData.bar->BackgroundColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
  _musicData.bar->FillColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
  _musicData.bar->FillRatio((_musicData.currentValue - _musicData.minValue)
                            / (_musicData.maxValue - _musicData.minValue));
  _musicData.bar->Horizontal(true);
  //_musicData.bar->Padding(glm::vec2(5.0f, 5.0f));
  _musicData.bar->Offset(glm::vec2(-75.0f, 100.0f));
  _musicData.text = std::make_shared< Engine::Renderer::Text >();
  _musicData.text->Size(1);
  _musicData.text->SetText("");
  _musicData.text->Color(glm::vec4(1.0f));
  _musicData.text->Offset(glm::vec2(-365.0f, 85.0f));
  _musicData.downButton = std::make_shared< Engine::Renderer::Button >();
  _musicData.downButton->Background(
      Engine::AssetManager::GetTexture2D("./textures/UI/but_minus.png"));
  _musicData.downButton->Size(glm::vec2(66.0f, 64.0f));
  _musicData.downButton->Color(glm::vec4(0.7f, 0.7f, 0.7f, 1.0f));
  _musicData.downButton->SelectedColor(glm::vec4(1.0f));
  _musicData.downButton->PressedColor(glm::vec4(0.6f, 0.6f, 0.6f, 1.0f));
  _musicData.downButton->Offset(glm::vec2(120.0f, 100.0f));
  _musicData.downButton->HandleSize(glm::vec2(66.0f, 64.0f));
  _musicData.downButton->text("");
  _musicData.downButton->TextColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
  _musicData.downButton->TextOffset(glm::vec2(-5.0f, -5.0f));
  // TODO: add value changing in game settings
  _musicData.downButton->OnPress([this]() {
    _musicData.currentValue =
        std::clamp(_musicData.currentValue - 1.0f, _musicData.minValue, _musicData.maxValue);
    _musicData.bar->FillRatio((_musicData.currentValue - _musicData.minValue)
                              / (_musicData.maxValue - _musicData.minValue));
  });
  _musicData.upButton = std::make_shared< Engine::Renderer::Button >();
  _musicData.upButton->Background(Engine::AssetManager::GetTexture2D("./textures/UI/but_plus.png"));
  _musicData.upButton->Size(glm::vec2(66.0f, 64.0f));
  _musicData.upButton->Color(glm::vec4(0.7f, 0.7f, 0.7f, 1.0f));
  _musicData.upButton->SelectedColor(glm::vec4(1.0f));
  _musicData.upButton->PressedColor(glm::vec4(0.6f, 0.6f, 0.6f, 1.0f));
  _musicData.upButton->Offset(glm::vec2(190.0f, 100.0f));
  _musicData.upButton->HandleSize(glm::vec2(66.0f, 64.0f));
  _musicData.upButton->text("");
  _musicData.upButton->TextColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
  _musicData.upButton->TextOffset(glm::vec2(-5.0f, -5.0f));
  // TODO: add value changing in game settings
  _musicData.upButton->OnPress([this]() {
    _musicData.currentValue =
        std::clamp(_musicData.currentValue + 1.0f, _musicData.minValue, _musicData.maxValue);
    _musicData.bar->FillRatio((_musicData.currentValue - _musicData.minValue)
                              / (_musicData.maxValue - _musicData.minValue));
  });

  _soundData.currentValue = 10.0f;
  _soundData.maxValue     = 100.0f;
  _soundData.minValue     = 0.0f;
  _soundData.bar          = std::make_shared< Engine::Renderer::Bar >();
  _soundData.bar->BackgroundTexture(Engine::AssetManager::GetTexture2D("./textures/UI/sounds.png"));
  _soundData.bar->FillTexture(Engine::AssetManager::GetTexture2D("./textures/UI/slider.png"));
  _soundData.bar->Size(glm::vec2(298.0f, 74.0f));
  _soundData.bar->FillAreaSize(glm::vec2(274.0f, 19.0f));
  _soundData.bar->FillAreaOffset(glm::vec2(0.0f, -20.0f));
  _soundData.bar->BackgroundColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
  _soundData.bar->FillColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
  _soundData.bar->FillRatio((_soundData.currentValue - _soundData.minValue)
                            / (_soundData.maxValue - _soundData.minValue));
  _soundData.bar->Horizontal(true);
  //_soundData.bar->Padding(glm::vec2(5.0f, 5.0f));
  _soundData.bar->Offset(glm::vec2(-75.0f, 25.0f));
  _soundData.text = std::make_shared< Engine::Renderer::Text >();
  _soundData.text->Size(1);
  _soundData.text->SetText("");
  _soundData.text->Color(glm::vec4(1.0f));
  _soundData.text->Offset(glm::vec2(-360.0f, 10.0f));
  _soundData.downButton = std::make_shared< Engine::Renderer::Button >();
  _soundData.downButton->Background(
      Engine::AssetManager::GetTexture2D("./textures/UI/but_minus.png"));
  _soundData.downButton->Size(glm::vec2(66.0f, 64.0f));
  _soundData.downButton->Color(glm::vec4(0.7f, 0.7f, 0.7f, 1.0f));
  _soundData.downButton->SelectedColor(glm::vec4(1.0f));
  _soundData.downButton->PressedColor(glm::vec4(0.6f, 0.6f, 0.6f, 1.0f));
  _soundData.downButton->Offset(glm::vec2(120.0f, 25.0f));
  _soundData.downButton->HandleSize(glm::vec2(66.0f, 64.0f));
  _soundData.downButton->text("");
  _soundData.downButton->TextColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
  _soundData.downButton->TextOffset(glm::vec2(-5.0f, -5.0f));
  // TODO: add value changing in game settings
  _soundData.downButton->OnPress([this]() {
    _soundData.currentValue =
        std::clamp(_soundData.currentValue - 1.0f, _soundData.minValue, _soundData.maxValue);
    _soundData.bar->FillRatio((_soundData.currentValue - _soundData.minValue)
                              / (_soundData.maxValue - _soundData.minValue));
  });
  _soundData.upButton = std::make_shared< Engine::Renderer::Button >();
  _soundData.upButton->Background(Engine::AssetManager::GetTexture2D("./textures/UI/but_plus.png"));
  _soundData.upButton->Size(glm::vec2(66.0f, 64.0f));
  _soundData.upButton->Color(glm::vec4(0.7f, 0.7f, 0.7f, 1.0f));
  _soundData.upButton->SelectedColor(glm::vec4(1.0f));
  _soundData.upButton->PressedColor(glm::vec4(0.6f, 0.6f, 0.6f, 1.0f));
  _soundData.upButton->Offset(glm::vec2(190.0f, 25.0f));
  _soundData.upButton->HandleSize(glm::vec2(66.0f, 64.0f));
  _soundData.upButton->text("");
  _soundData.upButton->TextColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
  _soundData.upButton->TextOffset(glm::vec2(-5.0f, -5.0f));
  // TODO: add value changing in game settings
  _soundData.upButton->OnPress([this]() {
    _soundData.currentValue =
        std::clamp(_soundData.currentValue + 1.0f, _soundData.minValue, _soundData.maxValue);
    _soundData.bar->FillRatio((_soundData.currentValue - _soundData.minValue)
                              / (_soundData.maxValue - _soundData.minValue));
  });

  _brightnessData.currentValue = 0.0f;  // GameManager::GetGameSettings()->Brightness();
  _brightnessData.maxValue     = 1.0f;
  _brightnessData.minValue     = -1.0f;
  _brightnessData.bar          = std::make_shared< Engine::Renderer::Bar >();
  _brightnessData.bar->BackgroundTexture(
      Engine::AssetManager::GetTexture2D("./textures/UI/brightness.png"));
  _brightnessData.bar->FillTexture(Engine::AssetManager::GetTexture2D("./textures/UI/slider.png"));
  _brightnessData.bar->Size(glm::vec2(298.0f, 74.0f));
  _brightnessData.bar->FillAreaSize(glm::vec2(274.0f, 19.0f));
  _brightnessData.bar->FillAreaOffset(glm::vec2(0.0f, -20.0f));
  _brightnessData.bar->BackgroundColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
  _brightnessData.bar->FillColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
  _brightnessData.bar->FillRatio((_brightnessData.currentValue - _brightnessData.minValue)
                                 / (_brightnessData.maxValue - _brightnessData.minValue));
  _brightnessData.bar->Horizontal(true);
  //_brightnessData.bar->Padding(glm::vec2(5.0f, 5.0f));
  _brightnessData.bar->Offset(glm::vec2(-75.0f, -50.0f));
  _brightnessData.text = std::make_shared< Engine::Renderer::Text >();
  _brightnessData.text->Size(1);
  _brightnessData.text->SetText("");
  _brightnessData.text->Color(glm::vec4(1.0f));
  _brightnessData.text->Offset(glm::vec2(-360.0f, -65.0f));
  _brightnessData.downButton = std::make_shared< Engine::Renderer::Button >();
  _brightnessData.downButton->Background(
      Engine::AssetManager::GetTexture2D("./textures/UI/but_minus.png"));
  _brightnessData.downButton->Size(glm::vec2(66.0f, 64.0f));
  _brightnessData.downButton->Color(glm::vec4(0.7f, 0.7f, 0.7f, 1.0f));
  _brightnessData.downButton->SelectedColor(glm::vec4(1.0f));
  _brightnessData.downButton->PressedColor(glm::vec4(0.6f, 0.6f, 0.6f, 1.0f));
  _brightnessData.downButton->Offset(glm::vec2(120.0f, -50.0f));
  _brightnessData.downButton->HandleSize(glm::vec2(66.0f, 64.0f));
  _brightnessData.downButton->text("");
  _brightnessData.downButton->TextColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
  _brightnessData.downButton->TextOffset(glm::vec2(-5.0f, -5.0f));
  // TODO: add value changing in game settings
  _brightnessData.downButton->OnPress([this]() {
    _brightnessData.currentValue = std::clamp(_brightnessData.currentValue - 0.1f,
                                              _brightnessData.minValue, _brightnessData.maxValue);
    _brightnessData.bar->FillRatio((_brightnessData.currentValue - _brightnessData.minValue)
                                   / (_brightnessData.maxValue - _brightnessData.minValue));
    GameManager::GetGameSettings()->Brightness(_brightnessData.currentValue);
  });
  _brightnessData.upButton = std::make_shared< Engine::Renderer::Button >();
  _brightnessData.upButton->Background(
      Engine::AssetManager::GetTexture2D("./textures/UI/but_plus.png"));
  _brightnessData.upButton->Size(glm::vec2(66.0f, 64.0f));
  _brightnessData.upButton->Color(glm::vec4(0.7f, 0.7f, 0.7f, 1.0f));
  _brightnessData.upButton->SelectedColor(glm::vec4(1.0f));
  _brightnessData.upButton->PressedColor(glm::vec4(0.6f, 0.6f, 0.6f, 1.0f));
  _brightnessData.upButton->Offset(glm::vec2(190.0f, -50.0f));
  _brightnessData.upButton->HandleSize(glm::vec2(66.0f, 64.0f));
  _brightnessData.upButton->text("");
  _brightnessData.upButton->TextColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
  _brightnessData.upButton->TextOffset(glm::vec2(-5.0f, -5.0f));
  // TODO: add value changing in game settings
  _brightnessData.upButton->OnPress([this]() {
    _brightnessData.currentValue = std::clamp(_brightnessData.currentValue + 0.1f,
                                              _brightnessData.minValue, _brightnessData.maxValue);
    _brightnessData.bar->FillRatio((_brightnessData.currentValue - _brightnessData.minValue)
                                   / (_brightnessData.maxValue - _brightnessData.minValue));
    GameManager::GetGameSettings()->Brightness(_brightnessData.currentValue);
  });

  _contrastData.currentValue = 1.0f;  // GameManager::GetGameSettings()->Contrast();
  _contrastData.maxValue     = 6.0f;
  _contrastData.minValue     = 0.5f;
  _contrastData.bar          = std::make_shared< Engine::Renderer::Bar >();
  _contrastData.bar->BackgroundTexture(
      Engine::AssetManager::GetTexture2D("./textures/UI/contrast.png"));
  _contrastData.bar->FillTexture(Engine::AssetManager::GetTexture2D("./textures/UI/slider.png"));
  _contrastData.bar->Size(glm::vec2(298.0f, 74.0f));
  _contrastData.bar->FillAreaSize(glm::vec2(274.0f, 19.0f));
  _contrastData.bar->FillAreaOffset(glm::vec2(0.0f, -20.0f));
  _contrastData.bar->BackgroundColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
  _contrastData.bar->FillColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
  _contrastData.bar->FillRatio((_contrastData.currentValue - _contrastData.minValue)
                               / (_contrastData.maxValue - _contrastData.minValue));
  _contrastData.bar->Horizontal(true);
  //_contrastData.bar->Padding(glm::vec2(5.0f, 5.0f));
  _contrastData.bar->Offset(glm::vec2(-75.0f, -125.0f));
  _contrastData.text = std::make_shared< Engine::Renderer::Text >();
  _contrastData.text->Size(1);
  _contrastData.text->SetText("");
  _contrastData.text->Color(glm::vec4(1.0f));
  _contrastData.text->Offset(glm::vec2(-360.0f, -140.0f));
  _contrastData.downButton = std::make_shared< Engine::Renderer::Button >();
  _contrastData.downButton->Background(
      Engine::AssetManager::GetTexture2D("./textures/UI/but_minus.png"));
  _contrastData.downButton->Size(glm::vec2(66.0f, 64.0f));
  _contrastData.downButton->Color(glm::vec4(0.7f, 0.7f, 0.7f, 1.0f));
  _contrastData.downButton->SelectedColor(glm::vec4(1.0f));
  _contrastData.downButton->PressedColor(glm::vec4(0.6f, 0.6f, 0.6f, 1.0f));
  _contrastData.downButton->Offset(glm::vec2(120.0f, -125.0f));
  _contrastData.downButton->HandleSize(glm::vec2(66.0f, 64.0f));
  _contrastData.downButton->text("");
  _contrastData.downButton->TextColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
  _contrastData.downButton->TextOffset(glm::vec2(-5.0f, -5.0f));
  // TODO: add value changing in game settings
  _contrastData.downButton->OnPress([this]() {
    _contrastData.currentValue = std::clamp(_contrastData.currentValue - 0.1f,
                                            _contrastData.minValue, _contrastData.maxValue);
    _contrastData.bar->FillRatio((_contrastData.currentValue - _contrastData.minValue)
                                 / (_contrastData.maxValue - _contrastData.minValue));
    GameManager::GetGameSettings()->Contrast(_contrastData.currentValue);
  });
  _contrastData.upButton = std::make_shared< Engine::Renderer::Button >();
  _contrastData.upButton->Background(
      Engine::AssetManager::GetTexture2D("./textures/UI/but_plus.png"));
  _contrastData.upButton->Size(glm::vec2(66.0f, 64.0f));
  _contrastData.upButton->Color(glm::vec4(0.7f, 0.7f, 0.7f, 1.0f));
  _contrastData.upButton->SelectedColor(glm::vec4(1.0f));
  _contrastData.upButton->PressedColor(glm::vec4(0.6f, 0.6f, 0.6f, 1.0f));
  _contrastData.upButton->Offset(glm::vec2(190.0f, -125.0f));
  _contrastData.upButton->HandleSize(glm::vec2(66.0f, 64.0f));
  _contrastData.upButton->text("");
  _contrastData.upButton->TextColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
  _contrastData.upButton->TextOffset(glm::vec2(-5.0f, -5.0f));
  // TODO: add value changing in game settings
  _contrastData.upButton->OnPress([this]() {
    _contrastData.currentValue = std::clamp(_contrastData.currentValue + 0.1f,
                                            _contrastData.minValue, _contrastData.maxValue);
    _contrastData.bar->FillRatio((_contrastData.currentValue - _contrastData.minValue)
                                 / (_contrastData.maxValue - _contrastData.minValue));
    GameManager::GetGameSettings()->Contrast(_contrastData.currentValue);
  });

  _gammaData.currentValue = 2.2f;  // GameManager::GetGameSettings()->Gamma();
  _gammaData.maxValue     = 8.0f;
  _gammaData.minValue     = 0.0f;
  _gammaData.bar          = std::make_shared< Engine::Renderer::Bar >();
  _gammaData.bar->BackgroundTexture(Engine::AssetManager::GetTexture2D("./textures/UI/gamma.png"));
  _gammaData.bar->FillTexture(Engine::AssetManager::GetTexture2D("./textures/UI/slider.png"));
  _gammaData.bar->Size(glm::vec2(298.0f, 74.0f));
  _gammaData.bar->FillAreaSize(glm::vec2(274.0f, 19.0f));
  _gammaData.bar->FillAreaOffset(glm::vec2(0.0f, -20.0f));
  _gammaData.bar->BackgroundColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
  _gammaData.bar->FillColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
  _gammaData.bar->FillRatio((_gammaData.currentValue - _gammaData.minValue)
                            / (_gammaData.maxValue - _gammaData.minValue));
  _gammaData.bar->Horizontal(true);
  //_gammaData.bar->Padding(glm::vec2(5.0f, 5.0f));
  _gammaData.bar->Offset(glm::vec2(-75.0f, -200.0f));
  _gammaData.text = std::make_shared< Engine::Renderer::Text >();
  _gammaData.text->Size(1);
  _gammaData.text->SetText("");
  _gammaData.text->Color(glm::vec4(1.0f));
  _gammaData.text->Offset(glm::vec2(-360.0f, -215.0f));
  _gammaData.downButton = std::make_shared< Engine::Renderer::Button >();
  _gammaData.downButton->Background(
      Engine::AssetManager::GetTexture2D("./textures/UI/but_minus.png"));
  _gammaData.downButton->Size(glm::vec2(66.0f, 64.0f));
  _gammaData.downButton->Color(glm::vec4(0.7f, 0.7f, 0.7f, 1.0f));
  _gammaData.downButton->SelectedColor(glm::vec4(1.0f));
  _gammaData.downButton->PressedColor(glm::vec4(0.6f, 0.6f, 0.6f, 1.0f));
  _gammaData.downButton->Offset(glm::vec2(120.0f, -200.0f));
  _gammaData.downButton->HandleSize(glm::vec2(66.0f, 64.0f));
  _gammaData.downButton->text("");
  _gammaData.downButton->TextColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
  _gammaData.downButton->TextOffset(glm::vec2(-5.0f, -5.0f));
  // TODO: add value changing in game settings
  _gammaData.downButton->OnPress([this]() {
    _gammaData.currentValue =
        std::clamp(_gammaData.currentValue - 0.1f, _gammaData.minValue, _gammaData.maxValue);
    _gammaData.bar->FillRatio((_gammaData.currentValue - _gammaData.minValue)
                              / (_gammaData.maxValue - _gammaData.minValue));
    GameManager::GetGameSettings()->Gamma(_gammaData.currentValue);
  });
  _gammaData.upButton = std::make_shared< Engine::Renderer::Button >();
  _gammaData.upButton->Background(Engine::AssetManager::GetTexture2D("./textures/UI/but_plus.png"));
  _gammaData.upButton->Size(glm::vec2(66.0f, 64.0f));
  _gammaData.upButton->Color(glm::vec4(0.7f, 0.7f, 0.7f, 1.0f));
  _gammaData.upButton->SelectedColor(glm::vec4(1.0f));
  _gammaData.upButton->PressedColor(glm::vec4(0.6f, 0.6f, 0.6f, 1.0f));
  _gammaData.upButton->Offset(glm::vec2(190.0f, -200.0f));
  _gammaData.upButton->HandleSize(glm::vec2(66.0f, 64.0f));
  _gammaData.upButton->text("");
  _gammaData.upButton->TextColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
  _gammaData.upButton->TextOffset(glm::vec2(-5.0f, -5.0f));
  // TODO: add value changing in game settings
  _gammaData.upButton->OnPress([this]() {
    _gammaData.currentValue =
        std::clamp(_gammaData.currentValue + 0.1f, _gammaData.minValue, _gammaData.maxValue);
    _gammaData.bar->FillRatio((_gammaData.currentValue - _gammaData.minValue)
                              / (_gammaData.maxValue - _gammaData.minValue));
    GameManager::GetGameSettings()->Gamma(_gammaData.currentValue);
  });

  _returnButton = std::make_shared< Engine::Renderer::Button >();
  _returnButton->Background(Engine::AssetManager::GetTexture2D("./textures/UI/back.png"));
  _returnButton->Size(glm::vec2(230.0f, 65.0f));
  _returnButton->Color(glm::vec4(0.7f, 0.7f, 0.7f, 1.0f));
  _returnButton->SelectedColor(glm::vec4(1.0f));
  _returnButton->PressedColor(glm::vec4(0.6f, 0.6f, 0.6f, 1.0f));
  _returnButton->Offset(glm::vec2(0.0f, -275.0f));
  _returnButton->HandleSize(glm::vec2(230.0f, 65.0f));
  _returnButton->text("");
  _returnButton->TextColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
  _returnButton->TextOffset(glm::vec2(-15.0f, -15.0f));
  _returnButton->OnPress([this]() { GameManager::SwitchScene(SceneName::MainMenu); });

  stbi_set_flip_vertically_on_load(false);

  // auto gui = Engine::ECS::EntityManager::GetInstance().GetSystem< Engine::Systems::GUISystem >();
  // gui->OnWindowResize(Engine::Window::Get().GetScreenSize());

  _scene->OnWindowResize(Engine::Window::Get().GetScreenSize());

  if (current_scene != nullptr) {
    Engine::SceneManager::OpenScene(current_scene->GetID());
  }
}

auto OptionsMenu::Show(std::function< void() > returnFunc) -> void {
  if (_isVisible)
      return;
  AddAllElements();
  _returnButton->OnPress([this, returnFunc]() {
    returnFunc();
    Hide();
  });
  _isVisible = true;
}

auto OptionsMenu::Hide() -> void {
  if (!_isVisible)
      return;
  RemoveAllElements();
  _isVisible = false;
}

auto OptionsMenu::Scene() -> std::shared_ptr< Engine::Scene > {
  return _scene;
}

auto OptionsMenu::AddAllElements() -> void {
  auto window_size          = Engine::Window::Get().GetScreenSize();  // glm::vec2(1600.0f, 900.0f);
  _backgroundEntity         = Engine::ECS::EntityManager::GetInstance().CreateEntity();
  auto backgroundUIRenderer = _backgroundEntity->AddComponent< Engine::Components::UIRenderer >();
  auto backgroundTransform  = _backgroundEntity->AddComponent< Engine::Transform >();

  backgroundTransform->Position(glm::vec3(window_size * 0.5f, 0.0f));
  backgroundUIRenderer->AddElement(_background);
  backgroundUIRenderer->AddElement(_title);

  _titleEntity         = Engine::ECS::EntityManager::GetInstance().CreateEntity();
  auto titleUIRenderer = _titleEntity->AddComponent< Engine::Components::UIRenderer >();
  auto titleTransform  = _titleEntity->AddComponent< Engine::Transform >();

  titleUIRenderer->AddElement(_titleImage);
  titleTransform->Position({window_size.x * 0.5f, window_size.y * 0.75f, 0.0f});

  _buttonsEntity         = Engine::ECS::EntityManager::GetInstance().CreateEntity();
  auto buttonsUIRenderer = _buttonsEntity->AddComponent< Engine::Components::UIRenderer >();
  auto buttonsTransform  = _buttonsEntity->AddComponent< Engine::Transform >();

  buttonsTransform->Position(glm::vec3(window_size * 0.5f, 0.0f));

  buttonsUIRenderer->AddElement(_musicData.downButton);
  buttonsUIRenderer->AddElement(_musicData.upButton);
  buttonsUIRenderer->AddElement(_musicData.bar);
  buttonsUIRenderer->AddElement(_musicData.text);
  buttonsUIRenderer->AddElement(_soundData.downButton);
  buttonsUIRenderer->AddElement(_soundData.upButton);
  buttonsUIRenderer->AddElement(_soundData.bar);
  buttonsUIRenderer->AddElement(_soundData.text);
  buttonsUIRenderer->AddElement(_brightnessData.downButton);
  buttonsUIRenderer->AddElement(_brightnessData.upButton);
  buttonsUIRenderer->AddElement(_brightnessData.bar);
  buttonsUIRenderer->AddElement(_brightnessData.text);
  buttonsUIRenderer->AddElement(_contrastData.downButton);
  buttonsUIRenderer->AddElement(_contrastData.upButton);
  buttonsUIRenderer->AddElement(_contrastData.bar);
  buttonsUIRenderer->AddElement(_contrastData.text);
  buttonsUIRenderer->AddElement(_gammaData.downButton);
  buttonsUIRenderer->AddElement(_gammaData.upButton);
  buttonsUIRenderer->AddElement(_gammaData.bar);
  buttonsUIRenderer->AddElement(_gammaData.text);
  buttonsUIRenderer->AddElement(_returnButton);

  buttonsUIRenderer->AddButton(_musicData.downButton);
  buttonsUIRenderer->AddButton(_musicData.upButton);
  buttonsUIRenderer->AddButton(_soundData.downButton);
  buttonsUIRenderer->AddButton(_soundData.upButton);
  buttonsUIRenderer->AddButton(_brightnessData.downButton);
  buttonsUIRenderer->AddButton(_brightnessData.upButton);
  buttonsUIRenderer->AddButton(_contrastData.downButton);
  buttonsUIRenderer->AddButton(_contrastData.upButton);
  buttonsUIRenderer->AddButton(_gammaData.downButton);
  buttonsUIRenderer->AddButton(_gammaData.upButton);
  buttonsUIRenderer->AddButton(_returnButton);
}

auto OptionsMenu::RemoveAllElements() -> void {
  Engine::ECS::EntityManager::GetInstance().RemoveEntity(_backgroundEntity);
  Engine::ECS::EntityManager::GetInstance().RemoveEntity(_buttonsEntity);
  Engine::ECS::EntityManager::GetInstance().RemoveEntity(_titleEntity);
}
