#pragma once
#include <pch.h>
#include <Components/Transform.h>

namespace Engine {
  class Camera {
  private:
    glm::mat4 _viewMatrix;
    glm::mat4 _projectionMatrix;
    float _fov;
    float _aspect;
    float _nearPlane;
    float _farPlane;

  public:
    Transform transform;
    Camera(float fov, float aspect, float nearPlane, float farPlane)
        : _fov(fov), _aspect(aspect), _nearPlane(nearPlane), _farPlane(farPlane) {
    }
    auto GetViewMatrix() noexcept -> glm::mat4 {
      // TODO: Check if dirty
      /*_viewMatrix = glm::lookAt(transform.Position(), transform.Position() + transform.Forward(),
                                {0.0f, 1.0f, 0.0f});*/
      _viewMatrix = glm::lookAt(transform.Position(), transform.Position() + transform.Forward(),
                                {0.0f, 1.0f, 0.0f});
      return _viewMatrix;
    }
    auto GetProjectionMatrix() noexcept -> glm::mat4 {
      _projectionMatrix = glm::perspective(_fov, _aspect, _nearPlane, _farPlane);
      return _projectionMatrix;
    }
  };
}  // namespace Engine