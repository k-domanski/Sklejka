#pragma once
#include <memory>

namespace Engine {
  namespace Renderer {
    class Image;
    class Text;
    class Button;
  }

  class Scene;
}

class LevelSelection
{
public:
  LevelSelection();
  ~LevelSelection() = default;
  auto Scene() -> std::shared_ptr< Engine::Scene >;

private:
  std::shared_ptr< Engine::Scene > _scene;
  std::shared_ptr< Engine::Renderer::Button > _lvl1Button;
  std::shared_ptr< Engine::Renderer::Button > _returnButton;
  std::shared_ptr< Engine::Renderer::Image > _lvl1Image;
  std::shared_ptr< Engine::Renderer::Image > _titleImage;
  std::shared_ptr< Engine::Renderer::Image > _background;
  std::shared_ptr< Engine::Renderer::Image > _buttonBackground;
};
