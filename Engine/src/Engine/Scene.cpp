#include "pch.h"
#include "Scene.h"
#include <Systems/Renderer.h>
#include <Systems/SceneGraph.h>
#include <Systems/CameraSystem.h>
#include <Systems/LightSystem.h>
#include "Systems/Physics.h"
//#include <ECS/ECS.h>
#include "ECS/EntityManager.h"

namespace Engine {
  auto Scene::GetID() -> size_t {
    return _id;
  }

  auto Scene::OpenScene() -> void {
    _sceneGraph    = ECS::EntityManager::GetInstance().RegisterSystem< Systems::SceneGraph >();
    _cameraSystem  = ECS::EntityManager::GetInstance().RegisterSystem< Systems::CameraSystem >();
    _renderSystem  = ECS::EntityManager::GetInstance().RegisterSystem< Systems::Renderer >();
    _physicsSystem = ECS::EntityManager::GetInstance().RegisterSystem< Systems::Physics >();
    _lightSystem = ECS::EntityManager::GetInstance().RegisterSystem< Systems::LightSystem>();
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
    _lightSystem->Update(deltaTime);
  }
  auto Scene::Draw() -> void {
    _renderSystem->Update(0.0f);
  }
  auto Scene::SceneGraph() -> std::shared_ptr< Systems::SceneGraph > {
    return _sceneGraph;
  }

  auto Scene::OnWindowResize(glm::vec2 windowSize) -> void {
    _renderSystem->OnWindowResize(windowSize);
  }
}  // namespace Engine
