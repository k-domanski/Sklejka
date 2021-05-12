#include "pch.h"
#include "UIRenderer.h"

std::vector< std::shared_ptr< Engine::Renderer::UIElement > >& Engine::Components::UIRenderer::GetElements() {
  return _elements;
}

Engine::Components::UIRenderer::UIRenderer(): Component("UIRenderer") {
}
