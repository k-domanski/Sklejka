#pragma once
#include <pch.h>
#include <Systems/Renderer.h>
#include <Systems/SceneGraph.h>
#include <Systems/CameraSystem.h>

#include "Systems/Physics.h"

namespace Engine {
  class Scene {
  private:
    std::shared_ptr< Systems::SceneGraph > _sceneGraph;
    std::shared_ptr< Systems::Renderer > _renderSystem;
    std::shared_ptr< Systems::CameraSystem > _cameraSystem;
    std::shared_ptr< Systems::Physics > _physicsSystem;

  public:
    Scene();
    auto Update(float deltaTime) -> void;
    auto Draw() -> void;
    auto SceneGraph() -> std::shared_ptr< Systems::SceneGraph >;
  };
}  // namespace Engine