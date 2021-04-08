#pragma once
#include <pch.h>
#include <ECS/ECS.h>
#include <Components/Flags.h>

namespace Engine {
  class Transform : public ECS::Component {
  private:
    glm::vec3 _position{0.0f};
    glm::quat _rotation{1.0f, 0.0f, 0.0f, 0.0f};
    glm::vec3 _scale{1.0f};
    glm::mat4 _modelMatrix;

  public:
    TransformBitFlags flags;
    Transform() = default;
    auto Position() const noexcept -> glm::vec3;
    auto Position(const glm::vec3& position) noexcept -> glm::vec3;
    auto Rotation() const noexcept -> glm::quat;
    auto Rotation(const glm::quat& rotation) noexcept -> glm::quat;
    auto Euler() const noexcept -> glm::vec3;
    auto Euler(const glm::vec3 angles) noexcept -> glm::quat;
    auto Scale() const noexcept -> glm::vec3;
    auto Scale(const glm::vec3& scale) noexcept -> glm::vec3;
    auto GetLocalMatrix() const noexcept -> glm::mat4;
    auto Right() const noexcept -> glm::vec3;
    auto Up() const noexcept -> glm::vec3;
    auto Forward() const noexcept -> glm::vec3;
    auto Rotate(float radians, const glm::vec3& axis) noexcept -> glm::quat;
  };
}  // namespace Engine