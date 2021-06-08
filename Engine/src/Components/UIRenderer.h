#pragma once
#include "ECS/Component.h"
#include "Renderer/Button.h"
#include "Renderer/Image.h"

namespace Engine::Components {
  class UIRenderer : public Engine::ECS::Component {
  public:
    UIRenderer();
    std::vector< std::shared_ptr< Engine::Renderer::UIElement > >& GetElements();
    std::vector< std::shared_ptr< Engine::Renderer::Button > >& GetButtons();
    auto AddButton(std::shared_ptr< Renderer::Button > button) -> void;
    auto RemoveButton(std::shared_ptr< Renderer::Button > button) -> void;
    auto AddElement(std::shared_ptr< Renderer::UIElement > element) -> void;
    auto RemoveElement(std::shared_ptr< Renderer::UIElement > element) -> void;
    auto SelectNext() -> void;
    auto SelectPrevious() -> void;
    auto SelectButton(int buttonNR) -> void;
    auto DeselectButton() -> void;
    auto TriggerOnPressed() -> void;

  private:
    std::vector< std::shared_ptr< Engine::Renderer::UIElement > > _elements;
    std::vector< std::shared_ptr< Engine::Renderer::Button > > _buttons;
    int _currentlySelectedButton;
  };
}  // namespace Engine::Components
