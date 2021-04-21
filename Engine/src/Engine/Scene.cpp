#include "pch.h"
#include "Scene.h"
#include <ECS/ECS.h>

namespace Engine {
  Scene::Scene() {
    using ECS::EntityManager;
    _sceneGraph    = EntityManager::GetInstance().RegisterSystem< Systems::SceneGraph >();
    _cameraSystem  = EntityManager::GetInstance().RegisterSystem< Systems::CameraSystem >();
    _renderSystem  = EntityManager::GetInstance().RegisterSystem< Systems::Renderer >();
    _physicsSystem = EntityManager::GetInstance().RegisterSystem< Systems::Physics >();
  }
  auto Scene::Update(float deltaTime) -> void {
    // Update other systems before
    _sceneGraph->Update(deltaTime);
    // Physics go here
    //
    _physicsSystem->Update(deltaTime);
    //
    // And after them scene graph again?
    _cameraSystem->Update(deltaTime);
  }
  auto Scene::Draw() -> void {
    _renderSystem->Update(0.0f);
  }
  auto Scene::SceneGraph() -> std::shared_ptr< Systems::SceneGraph > {
    return _sceneGraph;
  }
}  // namespace Engine
