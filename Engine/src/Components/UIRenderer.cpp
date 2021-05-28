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

auto Engine::Components::UIRenderer::AddButton(std::shared_ptr< Renderer::Button > button) -> void {
  _buttons.push_back(button);
}

auto Engine::Components::UIRenderer::RemoveButton(std::shared_ptr< Renderer::Button > button)
    -> void {
  auto it = std::find(_buttons.begin(), _buttons.end(), button);
  _buttons.erase(it);
}

auto Engine::Components::UIRenderer::AddElement(std::shared_ptr< Renderer::UIElement > element)
    -> void {
  _elements.push_back(element);
}

auto Engine::Components::UIRenderer::RemoveElement(std::shared_ptr< Renderer::UIElement > element)
    -> void {
  auto it = std::find(_elements.begin(), _elements.end(), element);
  _elements.erase(it);
}

Engine::Components::UIRenderer::UIRenderer(): Component("UIRenderer") {
}
