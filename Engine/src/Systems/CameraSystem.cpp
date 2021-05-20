#include "pch.h"
#include "CameraSystem.h"
#include <ECS/EntityManager.h>
#include <App/Log.h>
#include <App/Window.h>

namespace Engine::Systems {
  using ECS::EntityManager;
  CameraSystem::CameraSystem(): _cameraUniformSlot(1u), _mainCamera(nullptr) {
    using namespace GL;
    AddSignature< Camera >();
    AddSignature< Transform >();

    _editorView = false;
    /*auto size      = Window::Get().GetScreenSize();
    _renderTexture = std::make_shared< RenderTarget >(size.x, size.y);
    _renderTexture->AttachColor(0, std::make_shared< TextureAttachment >(
                                       size.x, size.y, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE));
    _renderTexture->AttachDepthStencil(std::make_shared< Renderbuffer >(size.x, size.y));*/
  }
  auto CameraSystem::Update(float deltaTime) -> void {
    // Update Cameras
    for (auto entityID : _entities) {
      auto transform = EntityManager::GetComponent< Transform >(entityID);
      auto camera    = EntityManager::GetComponent< Camera >(entityID);

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
      FindMainCamera();
    }
    if (_mainCamera != nullptr) {
      if (_mainCamera->flags.Get(CameraFlag::NewData)) {
        _cameraUniformBuffer.SetData(_mainCamera->UniformData());
      }
    }
    // Move to constructor?
    _cameraUniformBuffer.BindToSlot(_cameraUniformSlot);
  }
  auto CameraSystem::MainCamera() const noexcept -> std::shared_ptr< Camera > {
    return _mainCamera;
  }

  auto CameraSystem::SwitchView() -> void {
    _mainCamera = nullptr;
    _editorView = !_editorView;
  }
  auto CameraSystem::EditorView(bool enable) -> void {
    _editorView = enable;
  }
  auto CameraSystem::FindMainCamera() -> void {
    for (auto entityID : _entities) {
      auto camera = EntityManager::GetComponent< Camera >(entityID);
      if (camera
          && camera->flags.Get(_editorView ? CameraFlag::EditorCamera : CameraFlag::MainCamera)) {
        if (_mainCamera == nullptr) {
          _mainCamera = camera;
        } else {
          // Multiple main cameras - log warning and use first one
          LOG_WARN("Multiple Main Cameras detected");
        }
      }
    }
  }
}  // namespace Engine::Systems
