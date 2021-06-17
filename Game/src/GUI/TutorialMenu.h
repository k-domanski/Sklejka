#pragma once
#include <functional>
#include <memory>

namespace Engine {
  namespace ECS {
    class Entity;
  }

  namespace Components {
    class UIRenderer;
  }

  namespace Renderer {
    class Button;
    class Image;
  }

  class Scene;
}

class TutorialMenu
{
public:
  TutorialMenu();
  ~TutorialMenu() = default;
  auto Scene() -> std::shared_ptr< Engine::Scene >;
  auto IsVisible() -> bool;
  auto Show(std::function< void() > returnFunc) -> void;
  auto Hide() -> void;
  auto HideFromButton() -> void;
  auto AddAllElements() -> void;
  auto RemoveAllElements() -> void;

private:
  std::shared_ptr< Engine::ECS::Entity > _entity;
  std::shared_ptr< Engine::Scene > _scene;
  std::shared_ptr< Engine::Renderer::Image > _background;
  std::shared_ptr< Engine::Renderer::Button > _returnButton;
  bool _isVisible;
};
