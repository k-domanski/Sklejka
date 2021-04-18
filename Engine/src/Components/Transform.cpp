#include "pch.h"
#include "Transform.h"

namespace Engine {
  Transform::Transform()
      : Component("Transform"), _position(0.0f), _rotation(1.0f, 0.0f, 0.0f, 0.0f), _scale(1.0f),
        _modelMatrix(glm::mat4(1.0f)) {
    flags.Set(TransformFlag::Dirty);
  }
  auto Transform::Position() const noexcept -> glm::vec3 {
    return _position;
  }
  auto Transform::Position(const glm::vec3& position) noexcept -> glm::vec3 {
    flags.Set(TransformFlag::Dirty | TransformFlag::NewData);
    return _position = position;
  }
  auto Transform::Rotation() const noexcept -> glm::quat {
    return _rotation;
  }
  auto Transform::Rotation(const glm::quat& rotation) noexcept -> glm::quat {
    flags.Set(TransformFlag::Dirty | TransformFlag::NewData);
    return _rotation = rotation;
  }
  auto Transform::Euler() const noexcept -> glm::vec3 {
    return glm::eulerAngles(_rotation);
  }
  auto Transform::Euler(const glm::vec3 angles) noexcept -> glm::quat {
    return _rotation = glm::quat(angles);
  }
  auto Transform::Scale() const noexcept -> glm::vec3 {
    return _scale;
  }
  auto Transform::Scale(const glm::vec3& scale) noexcept -> glm::vec3 {
    flags.Set(TransformFlag::Dirty | TransformFlag::NewData);
    return _scale = scale;
  }
  auto Transform::GetLocalMatrix() const noexcept -> glm::mat4 {
    return glm::translate(glm::mat4(1.0f), _position) * glm::toMat4(_rotation)
           * glm::scale(glm::mat4(1.0f), _scale);
  }
  auto Transform::GetWorldMatrix() const noexcept -> glm::mat4 {
    return _modelMatrix;
  }
  auto Transform::Right() const noexcept -> glm::vec3 {
    // TODO: if dirty -> update
    return glm::normalize(static_cast< glm::mat3 >(_modelMatrix) * glm::vec3{1.0f, 0.0f, 0.0f});
    // return _rotation * glm::vec3{1.0f, 0.0f, 0.0f};
  }
  auto Transform::Up() const noexcept -> glm::vec3 {
    // TODO: if dirty -> update
    return glm::normalize(static_cast< glm::mat3 >(_modelMatrix) * glm::vec3{0.0f, 1.0f, 0.0f});
    // return _rotation * glm::vec3{0.0f, 1.0f, 0.0f};
  }
  auto Transform::Forward() const noexcept -> glm::vec3 {
    // TODO: if dirty -> update
    return glm::normalize(static_cast< glm::mat3 >(_modelMatrix) * glm::vec3{0.0f, 0.0f, 1.0f});
    // return _rotation * glm::vec3{0.0f, 0.0f, 1.0f};
  }
  auto Transform::Rotate(float radians, const glm::vec3& axis) noexcept -> glm::quat {
    flags.Set(TransformFlag::Dirty | TransformFlag::NewData);
    return _rotation = glm::rotate(glm::quat{1.0f, 0.0f, 0.0f, 0.0f}, radians, axis) * _rotation;
  }
}  // namespace Engine
