#pragma once
#include <pch.h>
#include <Systems/Renderer.h>
#include <Systems/SceneGraph.h>
#include <Systems/CameraSystem.h>

#include "Systems/Physics.h"

namespace Engine {
  class Scene {
  private:
    friend class ECS::EntityManager;
    std::shared_ptr< Systems::SceneGraph > _sceneGraph;
    std::shared_ptr< Systems::Renderer > _renderSystem;
    std::shared_ptr< Systems::CameraSystem > _cameraSystem;
    std::shared_ptr< Systems::Physics > _physicsSystem;

    size_t _id;
    std::vector< std::shared_ptr< ECS::Entity > > _entities;
    std::map< ECS::ComponentTypeID, std::shared_ptr< ECS::IComponentList > > _componentLists;
    std::map< ECS::SystemTypeID, std::shared_ptr< ECS::System > > _registeredSystems;

  public:
    Scene();
    Scene(size_t id): _id(id) {
      using ECS::EntityManager;
      _sceneGraph    = EntityManager::GetInstance().RegisterSystem< Systems::SceneGraph >();
      _renderSystem  = EntityManager::GetInstance().RegisterSystem< Systems::Renderer >();
      _cameraSystem  = EntityManager::GetInstance().RegisterSystem< Systems::CameraSystem >();
      _physicsSystem = EntityManager::GetInstance().RegisterSystem< Systems::Physics >();
    }
    auto GetID() -> size_t;
    auto OpenScene() -> void;
    auto Update(float deltaTime) -> void;
    auto Draw() -> void;
    auto SceneGraph() -> std::shared_ptr< Systems::SceneGraph >;
    auto OnWindowResize(glm::vec2 windowSize) -> void {
      _renderSystem->OnWindowResize(windowSize);
    }
  };
}  // namespace Engine