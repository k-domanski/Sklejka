#pragma once
#include <pch.h>
#include <ECS/ECS.h>
#include <Components/Camera.h>
#include <Components/Transform.h>
#include <GL/Buffer.h>


namespace Engine::Systems {
  class CameraSystem : public ECS::System {
  private:
    uint32_t _cameraUniformSlot;
    std::shared_ptr< Camera > _mainCamera;
    GL::CameraUniformBuffer _cameraUniformBuffer;

  public:
    CameraSystem();
    auto Update(float deltaTime) -> void override;
  };
}