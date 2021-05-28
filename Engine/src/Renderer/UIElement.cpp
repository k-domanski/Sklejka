#include "pch.h"
#include "UIElement.h"

auto Engine::Renderer::UIElement::SetActive(bool active) -> void {
  _isActive = active;
}

auto Engine::Renderer::UIElement::IsActive() -> bool {
  return _isActive;
}
