#pragma once
#include "Components/UIRenderer.h"
#include "Renderer/Button.h"
#include "Renderer/Image.h"

class EndLevelMenu {
public:
  EndLevelMenu();
  auto Show(bool win, float time, int bells) -> void;
  auto Hide() -> void;
  auto IsVisible() -> bool;
private:

  bool _win;
  bool _visible;
  //common
  std::shared_ptr< Engine::Components::UIRenderer > _imageRenderer;
  std::shared_ptr< Engine::Components::UIRenderer > _buttonRenderer;
  std::shared_ptr< Engine::Renderer::Image > _background;
  std::shared_ptr< Engine::Renderer::Image > _title;
  std::shared_ptr< Engine::Renderer::Button > _tryAgainButton;
  std::shared_ptr< Engine::Renderer::Button > _mainMenuButton;

  //win summary
  std::shared_ptr< Engine::Renderer::Text > _timeText;
  std::shared_ptr< Engine::Renderer::Text > _bellText;
  std::shared_ptr< Engine::Renderer::Button > _nextLevelButton;

  //lost summary
  std::shared_ptr< Engine::Renderer::Image > _deathMark;
  
};
