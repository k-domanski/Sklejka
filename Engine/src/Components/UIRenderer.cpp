#include "pch.h"
#include "UIRenderer.h"
#include <ECS/Types.h>

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

auto Engine::Components::UIRenderer::SelectNext() -> void {
  if (_buttons.empty())
    return;
  _buttons[_currentlySelectedButton]->IsSelected(false);
  _currentlySelectedButton++;
  if (_currentlySelectedButton < 0)
    _currentlySelectedButton = _buttons.size() - 1;
  _currentlySelectedButton %= _buttons.size();
  _buttons[_currentlySelectedButton]->IsSelected(true);
}

auto Engine::Components::UIRenderer::SelectPrevious() -> void {
  if (_buttons.empty())
    return;
  _buttons[_currentlySelectedButton]->IsSelected(false);
  _currentlySelectedButton--;
  if (_currentlySelectedButton < 0)
    _currentlySelectedButton = _buttons.size() - 1;
  _currentlySelectedButton %= _buttons.size();
  _buttons[_currentlySelectedButton]->IsSelected(true);
}

auto Engine::Components::UIRenderer::SelectButton(int buttonNR) -> void {
  if (_buttons.empty())
    return;
  _buttons[_currentlySelectedButton]->IsSelected(false);
  _currentlySelectedButton = buttonNR % _buttons.size();
  _buttons[_currentlySelectedButton]->IsSelected(true);
}

auto Engine::Components::UIRenderer::DeselectButton() -> void {
  if (_buttons.empty())
    return;
  _buttons[_currentlySelectedButton]->IsSelected(false);
}

auto Engine::Components::UIRenderer::TriggerOnPressed() -> void {
  if (_buttons.size() == 0)
    return;
  auto button = _buttons[_currentlySelectedButton];
  if (button->IsActive())
    button->TriggerOnPress();
}

Engine::Components::UIRenderer::UIRenderer()
    : Component("UIRenderer", ECS::GetComponentTypeID< UIRenderer >()) {
  _currentlySelectedButton = 0;
}