#include "pch.h"
#include "Scene.h"
#include <Systems/Renderer.h>
#include <Systems/SceneGraph.h>
#include <Systems/CameraSystem.h>
#include <Systems/LightSystem.h>
#include "Systems/Physics.h"
#include <Systems/ScriptSystem.h>
#include <Systems/NodeSystem.h>
//#include <ECS/ECS.h>
#include "ECS/EntityManager.h"
#include "Systems/GUISystem.h"

namespace Engine {
  Scene::Scene(const Scene& other) {
    _sceneGraph    = other._sceneGraph;
    _renderSystem  = other._renderSystem;
    _cameraSystem  = other._cameraSystem;
    _physicsSystem = other._physicsSystem;
    _lightSystem   = other._lightSystem;
    _scriptSystem  = other._scriptSystem;
    _GUISystem     = other._GUISystem;
    _nodeSystem    = other._nodeSystem;

    _id = other._id;

    for (auto entity : other._entities) {
      _entities.push_back(std::make_shared< ECS::Entity >(*entity));
    }

    for (auto [compType, list] : other._componentLists) {
      _componentLists[compType] = list->clone();
    }

    _registeredSystems = other._registeredSystems;
    //   reset systems here
    for (auto [systemID, system] : _registeredSystems) {
      system->ResetSystem();
      for (auto entity : _entities) {
        if (system->SignatureMatch(*entity->GetSignature()))
          system->AddEntity(entity->GetID());
      }
    }
  }

  //auto Scene::Clone() -> std::shared_ptr< Scene > {
  //  auto copy = std::make_shared< Scene >(_id);
  //  copy->_sceneGraph = _sceneGraph;
  //  copy->_renderSystem = _renderSystem;
  //  copy->_cameraSystem = _cameraSystem;
  //  copy->_physicsSystem = _physicsSystem;
  //  copy->_lightSystem   = _lightSystem;
  //  copy->_scriptSystem  = _scriptSystem;
  //  copy->_GUISystem     = _GUISystem;
  //  copy->_nodeSystem    = _nodeSystem;

  //    copy->_id = _id;

  //    for (auto entity : _entities) {
  //      copy->_entities.push_back(std::make_shared< ECS::Entity >(*entity));
  //    }

  //    for (auto [compType, list] : _componentLists) {
  //      copy->_componentLists[compType] = list->clone();
  //    }

  //    copy->_registeredSystems = _registeredSystems;
  //    //   reset systems here
  //    for (auto [systemID, system] : copy->_registeredSystems) {
  //      system->ResetSystem();
  //      for (auto entity : copy->_entities) {
  //        if (system->SignatureMatch(*entity->GetSignature()))
  //          system->AddEntity(entity->GetID());
  //      }
  //    }

  //  return copy;
  //}

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
    _GUISystem     = ECS::EntityManager::GetInstance().RegisterSystem< Systems::GUISystem >();
    _nodeSystem    = ECS::EntityManager::GetInstance().RegisterSystem< Engine::NodeSystem >();
    _cameraSystem->FindMainCamera();
  }

  auto Scene::Update(float deltaTime) -> void {
    // Update scripts
    _scriptSystem->Update(deltaTime);
    _nodeSystem->Update(deltaTime);

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
    _GUISystem->Update(0.0f);
  }
  auto Scene::SceneGraph() -> std::shared_ptr< Systems::SceneGraph > {
    return _sceneGraph;
  }

  auto Scene::CameraSystem() -> std::shared_ptr< Systems::CameraSystem > {
    return _cameraSystem;
  }

  auto Scene::RenderSystem() -> std::shared_ptr< Systems::Renderer > {
    return _renderSystem;
  }

  auto Scene::OnWindowResize(glm::vec2 windowSize) -> void {
    _renderSystem->OnWindowResize(windowSize);
    _GUISystem->OnWindowResize(windowSize);
  }

  auto Scene::OnMousePressed(glm::vec2 position) -> void {
    _GUISystem->HandleMousePressed(position);
  }

  auto Scene::OnMouseReleased(glm::vec2 position) -> void {
    _GUISystem->HandleMouseRelease(position);
  }

  auto Scene::FindEntity(const std::string& name) -> std::shared_ptr< ECS::Entity > {
    auto it = std::find_if(_entities.begin(), _entities.end(),
                           [&name](const auto& entity) { return entity->Name() == name; });
    if (it == _entities.end()) {
      return nullptr;
    }
    return *it;
  }
  auto Scene::NodeSystem() const -> std::shared_ptr< Engine::NodeSystem > {
    return _nodeSystem;
  }
}  // namespace Engine
