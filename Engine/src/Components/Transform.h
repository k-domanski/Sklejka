#pragma once
#include <pch.h>
#include <ECS/ECS.h>
#include <Utility/Utility.h>

namespace Engine::Systems {
  class SceneGraph;
}

namespace Engine {
  BETTER_ENUM(__TransformFlag, uint32_t, Dirty = Helpers::Bit32(0), NewData = Helpers::Bit32(1));
  typedef __TransformFlag TransformFlag;
  typedef Utility::BitFlags< TransformFlag > TransformBitFlags;

  class Transform : public ECS::Component {
  private:
    glm::vec3 _position{0.0f};
    glm::quat _rotation{1.0f, 0.0f, 0.0f, 0.0f};
    glm::vec3 _scale{1.0f};
    glm::mat4 _modelMatrix;

  public:
    friend class Engine::Systems::SceneGraph;
    TransformBitFlags flags;
    Transform();
    auto Position() const noexcept -> glm::vec3;
    auto Position(const glm::vec3& position) noexcept -> glm::vec3;
    auto Rotation() const noexcept -> glm::quat;
    auto Rotation(const glm::quat& rotation) noexcept -> glm::quat;
    auto Euler() const noexcept -> glm::vec3;
    auto Euler(const glm::vec3 angles) noexcept -> glm::quat;
    auto Scale() const noexcept -> glm::vec3;
    auto Scale(const glm::vec3& scale) noexcept -> glm::vec3;
    auto GetLocalMatrix() const noexcept -> glm::mat4;
    auto GetWorldMatrix() const noexcept -> glm::mat4;
    auto Right() const noexcept -> glm::vec3;
    auto Up() const noexcept -> glm::vec3;
    auto Forward() const noexcept -> glm::vec3;
    auto Rotate(float radians, const glm::vec3& axis) noexcept -> glm::quat;
    auto WorldPosition() const noexcept -> glm::vec3;
    auto WorlScale() const noexcept -> glm::vec3;
    std::string SaveToJson(std::string filePath) override;
    auto LoadFromJson(std::string filePath) -> void override;
  };
}  // namespace Engine