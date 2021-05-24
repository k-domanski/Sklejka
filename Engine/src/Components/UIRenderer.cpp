#include "pch.h"
#include "UIRenderer.h"

std::vector< std::shared_ptr< Engine::Renderer::UIElement > >&
    Engine::Components::UIRenderer::GetElements() {
  return _elements;
}

std::vector< std::shared_ptr< Engine::Renderer::Button > >&
    Engine::Components::UIRenderer::GetButtons() {
  return _buttons;
}

Engine::Components::UIRenderer::UIRenderer(): Component("UIRenderer") {
}
