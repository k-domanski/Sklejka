#pragma once
#include <memory>

namespace Engine {
  class Scene;
}

namespace Engine {
  namespace Renderer {
    class Button;
    class Image;
  }  // namespace Renderer
}  // namespace Engine

class CreditsMenu {
public:
  CreditsMenu();
  ~CreditsMenu() = default;
  auto Scene() -> std::shared_ptr< Engine::Scene >;

private:
  std::shared_ptr< Engine::Scene > _scene;
  std::shared_ptr< Engine::Renderer::Image > _image;
  std::shared_ptr< Engine::Renderer::Button > _exitButton;
};
