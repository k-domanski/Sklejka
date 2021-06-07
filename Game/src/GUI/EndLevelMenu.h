#pragma once
#include "Components/UIRenderer.h"
#include "Renderer/Button.h"
#include "Renderer/Image.h"

class EndLevelMenu {
public:
  EndLevelMenu();
  auto Show(std::string timeText, std::string winText) -> void;
  auto Hide() -> void;
private:
  std::shared_ptr< Engine::Components::UIRenderer > _imageRenderer;
  std::shared_ptr< Engine::Renderer::Image > _image;
  std::shared_ptr< Engine::Components::UIRenderer > _tryAgainRenderer;
  std::shared_ptr< Engine::Renderer::Button > _tryAgainButton;
  std::shared_ptr< Engine::Components::UIRenderer > _nextLevelRenderer;
  std::shared_ptr< Engine::Renderer::Button > _nextLevelButton;
  std::shared_ptr< Engine::Renderer::Text > _timeText;
  std::shared_ptr< Engine::Renderer::Text > _winText;
};
