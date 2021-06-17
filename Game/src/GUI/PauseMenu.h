#pragma once
#include <memory>

#include "Components/UIRenderer.h"
#include "Renderer/Button.h"
#include "Renderer/Image.h"

class PauseMenu
{
public:
  PauseMenu();
  auto Show(bool pause = true) -> void;
  auto Hide(bool unpause = true) -> void;
  ~PauseMenu();
private:
  std::shared_ptr< Engine::Renderer::Image > _background;
  std::shared_ptr< Engine::Renderer::Image > _title;
  std::shared_ptr< Engine::Renderer::Button > _resumeButton;
  std::shared_ptr< Engine::Renderer::Button > _mainMenuButton;
  std::shared_ptr< Engine::Renderer::Button > _optionsButton;
  std::shared_ptr< Engine::Renderer::Button > _tutorialButton;
  std::shared_ptr< Engine::Components::UIRenderer > _backroundRenderer;
  std::shared_ptr< Engine::Components::UIRenderer > _buttonRenderer;
  float _playerTimeScaleBeforePausing = 1.f;
  bool _isVisible;
};
