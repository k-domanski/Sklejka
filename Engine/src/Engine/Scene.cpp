#include "pch.h"
#include "Scene.h"
#include <ECS/ECS.h>

namespace Engine {
  Scene::Scene() {
    using ECS::EntityManager;
    _sceneGraph   = EntityManager::GetInstance().RegisterSystem< Systems::SceneGraph >();
    _renderSystem = EntityManager::GetInstance().RegisterSystem< Systems::Renderer >();
    _cameraSystem = EntityManager::GetInstance().RegisterSystem< Systems::CameraSystem >();
  }
  auto Scene::Update(float deltaTime) -> void {
    // Update other systems before
    _sceneGraph->Update();
    // Physics go here
    // And after them scene graph again?
    _cameraSystem->Update();
  }
  auto Scene::Draw() -> void {
    _renderSystem->Update();
  }
  auto Scene::SceneGraph() -> std::shared_ptr< Systems::SceneGraph > {
    return _sceneGraph;
  }
}  // namespace Engine
