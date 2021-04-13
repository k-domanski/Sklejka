#pragma once
#include <pch.h>
#include <Systems/Renderer.h>
#include <Systems/SceneGraph.h>

namespace Engine {
  class Scene {
  private:
    std::shared_ptr< Systems::SceneGraph > _sceneGraph;
    std::shared_ptr< Systems::Renderer > _renderSystem;

  public:
    Scene();
    auto Update(float deltaTime) -> void;
    auto Draw() -> void;
    auto SceneGraph() -> std::shared_ptr< Systems::SceneGraph >;
  };
}  // namespace Engine