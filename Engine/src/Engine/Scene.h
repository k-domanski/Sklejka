#pragma once
#include "ECS/Types.h"
#include "Systems/GUISystem.h"
//#include "ECS/EntityManager.h"
//#include <pch.h>

namespace Engine {
  namespace ECS {
    class Entity;
    class IComponentList;
    class System;
  }  // namespace ECS

  namespace Systems {
    class SceneGraph;
    class Renderer;
    class CameraSystem;
    class Physics;
    class LightSystem;
    class ScriptSystem;
  }  // namespace Systems

  namespace ECS {
    class EntityManager;
  }
  class Scene {
  private:
    friend ECS::EntityManager;
    std::shared_ptr< Systems::SceneGraph > _sceneGraph;
    std::shared_ptr< Systems::Renderer > _renderSystem;
    std::shared_ptr< Systems::CameraSystem > _cameraSystem;
    std::shared_ptr< Systems::Physics > _physicsSystem;
    std::shared_ptr< Systems::LightSystem > _lightSystem;
    std::shared_ptr< Systems::ScriptSystem > _scriptSystem;
    std::shared_ptr< Systems::GUISystem > _GUISystem;

    size_t _id;
    std::vector< std::shared_ptr< ECS::Entity > > _entities;
    std::map< ECS::ComponentTypeID, std::shared_ptr< ECS::IComponentList > > _componentLists;
    std::map< ECS::SystemTypeID, std::shared_ptr< ECS::System > > _registeredSystems;

  public:
    Scene(size_t id): _id(id) {
      /*using ECS::EntityManager;
      _sceneGraph    = EntityManager::GetInstance().RegisterSystem< Systems::SceneGraph >();
      _renderSystem  = EntityManager::GetInstance().RegisterSystem< Systems::Renderer >();
      _cameraSystem  = EntityManager::GetInstance().RegisterSystem< Systems::CameraSystem >();
      _physicsSystem = EntityManager::GetInstance().RegisterSystem< Systems::Physics >();*/
    }
    auto GetID() -> size_t;
    auto OpenScene() -> void;
    auto Update(float deltaTime) -> void;
    auto Draw() -> void;
    auto SceneGraph() -> std::shared_ptr< Systems::SceneGraph >;
    auto CameraSystem() -> std::shared_ptr< Systems::CameraSystem >;
    auto OnWindowResize(glm::vec2 windowSize) -> void;
    auto FindEntity(const std::string& name) -> std::shared_ptr< ECS::Entity >;
  };
}  // namespace Engine