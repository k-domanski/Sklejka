#include "pch.h"
#include "Scene.h"
#include <Systems/Renderer.h>
#include <Systems/SceneGraph.h>
#include <Systems/CameraSystem.h>
#include <Systems/LightSystem.h>
#include "Systems/Physics.h"
#include <Systems/ScriptSystem.h>
//#include <ECS/ECS.h>
#include "ECS/EntityManager.h"

namespace Engine {
  auto Scene::GetID() -> size_t {
    return _id;
  }

  auto Scene::OpenScene() -> void {
    _sceneGraph    = ECS::EntityManager::GetInstance().RegisterSystem< Systems::SceneGraph >();
    _lightSystem   = ECS::EntityManager::GetInstance().RegisterSystem< Systems::LightSystem >();
    _cameraSystem  = ECS::EntityManager::GetInstance().RegisterSystem< Systems::CameraSystem >();
    _renderSystem  = ECS::EntityManager::GetInstance().RegisterSystem< Systems::Renderer >();
    _physicsSystem = ECS::EntityManager::GetInstance().RegisterSystem< Systems::Physics >();
    _scriptSystem  = ECS::EntityManager::GetInstance().RegisterSystem< Systems::ScriptSystem >();
  }

  auto Scene::Update(float deltaTime) -> void {
    // Update scripts
    _scriptSystem->Update(deltaTime);

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

  auto Scene::CameraSystem() -> std::shared_ptr< Systems::CameraSystem > {
    return _cameraSystem;
  }

  auto Scene::OnWindowResize(glm::vec2 windowSize) -> void {
    _renderSystem->OnWindowResize(windowSize);
  }
  auto Scene::FindEntity(const std::string& name) -> std::shared_ptr< ECS::Entity > {
    auto it = std::find_if(_entities.begin(), _entities.end(),
                           [&name](const auto& entity) { return entity->Name() == name; });
    if (it == _entities.end()) {
      return nullptr;
    }
    return *it;
  }
}  // namespace Engine
