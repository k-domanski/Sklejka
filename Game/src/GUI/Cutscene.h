#pragma once
#include <memory>

namespace Engine {
  class Scene;
  namespace Renderer {
    class Image;
  }
}  // namespace Engine

class Cutscene {
private:
  std::shared_ptr< Engine::Renderer::Image > _image;
  std::shared_ptr< Engine::Scene > _scene;
  std::shared_ptr< class CutscenePlayer > _cutscenePlayer;

public:
  Cutscene();
  auto Scene() -> std::shared_ptr< Engine::Scene >;
  auto Reset() -> void;
};