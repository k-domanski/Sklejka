#pragma once
#include <memory>



#include "Components/UIRenderer.h"
#include "Renderer/Button.h"
#include "Renderer/Image.h"

class PauseMenu
{
public:
  PauseMenu();
  auto Show() -> void;
  auto Hide() -> void;
  ~PauseMenu();
private:
  std::shared_ptr< Engine::Renderer::Image > _background;
  std::shared_ptr< Engine::Renderer::Text > _text;
  std::shared_ptr< Engine::Renderer::Button > _resumeButton;
  std::shared_ptr< Engine::Renderer::Button > _mainMenuButton;
  std::shared_ptr< Engine::Components::UIRenderer > _backroundRenderer;
  std::shared_ptr< Engine::Components::UIRenderer > _mainMenuButtonRenderer;
  std::shared_ptr< Engine::Components::UIRenderer > _resumeButtonRenderer;
  float _playerTimeScaleBeforePausing = 1.f;
  bool _isVisible;
};
