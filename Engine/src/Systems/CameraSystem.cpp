#include "pch.h"
#include "CameraSystem.h"
#include <ECS/EntityManager.h>
#include <App/Log.h>

namespace Engine::Systems {
  using ECS::EntityManager;
  CameraSystem::CameraSystem()
      : _cameraUniformSlot(1u), _mainCamera(nullptr) {
    AddSignature< Camera >();
    AddSignature< Transform >();
  }
  auto CameraSystem::Update() -> void {
    // Update Cameras
    for (auto entityID : _entities) {
      auto transform = EntityManager::GetInstance().GetComponent< Transform >(entityID);
      auto camera    = EntityManager::GetInstance().GetComponent< Camera >(entityID);

      if (transform->flags.GetAll(TransformFlag::Dirty | TransformFlag::NewData)) {
        // This is transform with changed data but is not updated - error
        LOG_WARN("Camera matrices are Dirty on update for Entity ID: {}", entityID);
      }
      if (transform->flags.Get(TransformFlag::NewData)) {
        camera->ViewMatrix(glm::lookAt(transform->Position(),
                                       transform->Position() + transform->Forward(),
                                       {0.0f, 1.0f, 0.0f}));
      }
      if (camera->flags.Get(CameraFlag::Dirty)) {
        camera->flags.Clear(CameraFlag::Dirty);
        camera->ProjectionMatrix(glm::perspective(camera->Fov(), camera->Aspect(),
                                                  camera->NearPlane(), camera->FarPlane()));
      }
    }

    // Find main camera
    if (_mainCamera == nullptr) {
      for (auto entityID : _entities) {
        auto camera = EntityManager::GetInstance().GetComponent< Camera >(entityID);
        if (camera->flags.Get(CameraFlag::MainCamera)) {
          if (_mainCamera == nullptr) {
            _mainCamera = camera;
          } else {
            // Multiple main cameras - log warning and use first one
            LOG_WARN("Multiple Main Cameras detected");
          }
        }
      }
    }

    if (_mainCamera->flags.Get(CameraFlag::NewData)) {
      _cameraUniformBuffer.SetData(_mainCamera->UniformData());
    }
    _cameraUniformBuffer.BindToSlot(_cameraUniformSlot);
  }
}  // namespace Engine::Systems