#pragma once
#include "ECS/Component.h"
#include "Renderer/Image.h"

namespace Engine::Components
{
  class UIRenderer : public Engine::ECS::Component {
  public:
    UIRenderer();
    std::vector< std::shared_ptr< Engine::Renderer::UIElement > >& GetElements();

  private:
    std::vector< std::shared_ptr< Engine::Renderer::UIElement > > _elements;
  };
}
