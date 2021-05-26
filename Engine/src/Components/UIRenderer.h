#pragma once
#include "ECS/Component.h"
#include "Renderer/Button.h"
#include "Renderer/Image.h"

namespace Engine::Components
{
  class UIRenderer : public Engine::ECS::Component {
  public:
    UIRenderer();
    std::vector< std::shared_ptr< Engine::Renderer::UIElement > >& GetElements();
    std::vector< std::shared_ptr< Engine::Renderer::Button > >& GetButtons();

  private:
    std::vector< std::shared_ptr< Engine::Renderer::UIElement > > _elements;
    std::vector< std::shared_ptr< Engine::Renderer::Button > > _buttons;
  };
}
