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
#include "Systems/AnimationSystem.h"
#include "Systems/ParticleSystem.h"

namespace Engine {
  auto Scene::GetID() -> size_t {
    return _id;
  }

  auto Scene::OpenScene() -> void {
    _sceneGraph     = ECS::EntityManager::GetInstance().RegisterSystem< Systems::SceneGraph >();
    _lightSystem    = ECS::EntityManager::GetInstance().RegisterSystem< Systems::LightSystem >();
    _cameraSystem   = ECS::EntityManager::GetInstance().RegisterSystem< Systems::CameraSystem >();
    _particleSystem = ECS::EntityManager::GetInstance().RegisterSystem< Systems::ParticleSystem >();
    _renderSystem   = ECS::EntityManager::GetInstance().RegisterSystem< Systems::Renderer >();
    _physicsSystem  = ECS::EntityManager::GetInstance().RegisterSystem< Systems::Physics >();
    _scriptSystem   = ECS::EntityManager::GetInstance().RegisterSystem< Systems::ScriptSystem >();
    _GUISystem      = ECS::EntityManager::GetInstance().RegisterSystem< Systems::GUISystem >();
    _nodeSystem     = ECS::EntityManager::GetInstance().RegisterSystem< Engine::NodeSystem >();
    _animationSystem =
        ECS::EntityManager::GetInstance().RegisterSystem< Systems::AnimationSystem >();
    _cameraSystem->FindMainCamera();
  }

  auto Scene::Update(float deltaTime) -> void {
    // Update scripts
    _scriptSystem->Update(deltaTime);
    _nodeSystem->Update(deltaTime);

    // Update other systems before
    _sceneGraph->ClearFlags();
    _sceneGraph->Update(deltaTime);
    // Physics go here
    //
    _physicsSystem->Update(deltaTime);
    //
    // And after them scene graph again?
    _cameraSystem->Update(deltaTime);
    _lightSystem->Update(deltaTime);
    _animationSystem->Update(deltaTime);

    _particleSystem->Update(deltaTime);
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
    if (_cameraSystem->MainCamera() != nullptr)
        _cameraSystem->MainCamera()->Aspect(Window::Get().GetAspectRatio());
    _GUISystem->OnWindowResize(windowSize);
  }

  auto Scene::OnMousePressed(glm::vec2 position) -> void {
    _GUISystem->HandleMousePressed(position);
  }

  auto Scene::OnMouseReleased(glm::vec2 position) -> void {
    _GUISystem->HandleMouseRelease(position);
  }

  auto Scene::OnKeyPressed(Key key) -> void {
    _scriptSystem->OnKeyPressed(key);
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
  auto Scene::Entities() -> std::vector< std::shared_ptr< ECS::Entity > > {
    return _entities;
  }
  auto Scene::ParticleSystem() -> std::shared_ptr< Systems::ParticleSystem > {
    return _particleSystem;
  }
}  // namespace Engine
