#include "pch.h"
#include "Scene.h"
#include <ECS/ECS.h>

namespace Engine {
  Scene::Scene() {
    using ECS::EntityManager;
    _sceneGraph   = EntityManager::GetInstance().RegisterSystem< Systems::SceneGraph >();
    _renderSystem = EntityManager::GetInstance().RegisterSystem< Systems::Renderer >();
  }
  auto Scene::Update(float deltaTime) -> void {
    // Update other systems before
    _sceneGraph->Update();
  }
  auto Scene::Draw() -> void {
    _renderSystem->Update();
  }
  auto Scene::SceneGraph() -> std::shared_ptr< Systems::SceneGraph > {
    return _sceneGraph;
  }
}  // namespace Engine
