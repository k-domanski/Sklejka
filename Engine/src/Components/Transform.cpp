#include "pch.h"
#include "Transform.h"

namespace Engine {
  auto Transform::Position() const noexcept -> glm::vec3 {
    return _position;
  }
  auto Transform::Position(const glm::vec3& position) noexcept -> glm::vec3 {
    flags.SetFlag(TransformFlags::Dirty);
    return _position = position;
  }
  auto Transform::Rotation() const noexcept -> glm::quat {
    return _rotation;
  }
  auto Transform::Rotation(const glm::quat& rotation) noexcept -> glm::quat {
    flags.SetFlag(TransformFlags::Dirty);
    return _rotation = rotation;
  }
  auto Transform::Scale() const noexcept -> glm::vec3 {
    return _scale;
  }
  auto Transform::Scale(const glm::vec3& scale) noexcept -> glm::vec3 {
    flags.SetFlag(TransformFlags::Dirty);
    return _scale = scale;
  }
  auto Transform::GetLocalMatrix() const noexcept -> glm::mat4 {
    return glm::translate(glm::toMat4(_rotation) * glm::scale(glm::mat4{1.0f}, _scale), _position);
  }
  auto Transform::Right() const noexcept -> glm::vec3 {
    return _rotation * glm::vec3{1.0f, 0.0f, 0.0f};
  }
  auto Transform::Up() const noexcept -> glm::vec3 {
    return _rotation * glm::vec3{0.0f, 1.0f, 0.0f};
  }
  auto Transform::Forward() const noexcept -> glm::vec3 {
    return _rotation * glm::vec3{0.0f, 0.0f, 1.0f};
  }
  auto Transform::Rotate(float radians, const glm::vec3& axis) noexcept -> glm::quat {
    return _rotation = glm::rotate(glm::quat{1.0f, 0.0f, 0.0f, 0.0f}, radians, axis) * _rotation;
  }
}  // namespace Engine
