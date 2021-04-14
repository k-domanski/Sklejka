#pragma once
#include <pch.h>
#include <Components/Transform.h>
#include <ECS/ECS.h>
#include <App/Window.h>
#include <GL/UniformBufferData.h>

namespace Engine {
  using Helpers::Bit32;
  BETTER_ENUM(__CameraFlag, uint32_t, Dirty = Bit32(0), NewData = Bit32(1), MainCamera = Bit32(2),
              EditorCamera = Bit32(3));
  typedef __CameraFlag CameraFlag;
  typedef Utility::BitFlags< CameraFlag > CameraBitFlags;

  class Camera : public ECS::Component {
  private:
    float _fov;
    float _aspect;
    float _nearPlane;
    float _farPlane;
    GL::CameraUniformData _matrices;

  public:
    CameraBitFlags flags;
    Camera(float fov, float aspect, float nearPlane, float farPlane);
    auto Fov() const noexcept -> float;
    auto Fov(float fov) noexcept -> float;
    auto Aspect() const noexcept -> float;
    auto Aspect(float aspect) noexcept -> float;
    auto NearPlane() const noexcept -> float;
    auto NearPlane(float nearPlane) noexcept -> float;
    auto FarPlane() const noexcept -> float;
    auto FarPlane(float farPlane) noexcept -> float;
    auto ViewMatrix() const noexcept -> glm::mat4;
    auto ViewMatrix(const glm::mat4& viewMatrix) noexcept -> glm::mat4;
    auto ProjectionMatrix() const noexcept -> glm::mat4;
    auto ProjectionMatrix(const glm::mat4& projectionMatrix) noexcept -> glm::mat4;
    auto UniformData() const noexcept -> GL::CameraUniformData;
  };
}  // namespace Engine