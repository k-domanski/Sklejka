#include "pch.h"
#include "Transform.h"
#include <ECS/Types.h>
#include <nlohmann/json.hpp>
#include <Engine.h>
#include <Systems/SceneGraph.h>

#include "glm/gtx/matrix_decompose.hpp"

namespace Engine {
  Transform::Transform()
      : Component("Transform", ECS::GetComponentTypeID< Transform >()), _position(0.0f),
        _rotation(1.0f, 0.0f, 0.0f, 0.0f), _scale(1.0f), _modelMatrix(glm::mat4(1.0f)) {
    flags.Set(TransformFlag::Dirty);
  }
  auto Transform::Position() const noexcept -> glm::vec3 {
    return _position;
  }
  auto Transform::Position(const glm::vec3& position) noexcept -> glm::vec3 {
    flags.Set(TransformFlag::Dirty);
    return _position = position;
  }
  auto Transform::Rotation() const noexcept -> glm::quat {
    return _rotation;
  }
  auto Transform::Rotation(const glm::quat& rotation) noexcept -> glm::quat {
    flags.Set(TransformFlag::Dirty);
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
    flags.Set(TransformFlag::Dirty);
    return _scale = scale;
  }
  auto Transform::GetLocalMatrix() const noexcept -> glm::mat4 {
    return glm::translate(glm::mat4(1.0f), _position) * glm::toMat4(_rotation)
           * glm::scale(glm::mat4(1.0f), _scale);
  }

  auto Transform::SetLocalMatrix(glm::mat4 matrix) -> void {
    glm::vec3 scale{1.0f};
    glm::quat rotation{1.0f, 0.0f, 0.0f, 0.0f};
    glm::vec3 translation{0.0f};
    glm::vec3 skew;
    glm::vec4 perspective;
    if (!glm::decompose(matrix, scale, rotation, translation, skew, perspective)) {
      translation = matrix[3];
      rotation    = {1.0f, 0.0f, 0.0f, 0.0f};
      // scale = { 0.1f, 0.1f, 0.1f };
    }

    Position(translation);
    Rotation(rotation);
    Scale(scale);
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
  auto Transform::Forward(const glm::vec3& forward) noexcept -> glm::vec3 {
    flags.Set(TransformFlag::Dirty);
    _modelMatrix[2] = glm::vec4(forward, 0.0f);
    _rotation       = glm::quat_cast(static_cast< glm::mat3 >(_modelMatrix));
    return forward;
  }
  auto Transform::Rotate(float radians, const glm::vec3& axis) noexcept -> glm::quat {
    flags.Set(TransformFlag::Dirty);
    return _rotation = glm::rotate(glm::quat{1.0f, 0.0f, 0.0f, 0.0f}, radians, axis) * _rotation;
  }

  auto Transform::WorldPosition() -> glm::vec3 {
    //if (flags.GetAll(TransformFlag::Dirty)) {
    //  // Trigger force update
    //  ForceUpdate();
    //}
    return glm::vec3(_modelMatrix[3]);
  }

  auto Transform::WorldScale() const noexcept -> glm::vec3 {
    glm::vec3 scale;
    glm::quat rotation;
    glm::vec3 translation;
    glm::vec3 skew;
    glm::vec4 perspective;
    glm::decompose(_modelMatrix, scale, rotation, translation, skew, perspective);

    return scale;
  }

  std::string Transform::SaveToJson(std::string filePath) {
    std::ofstream ofstream;
    ofstream.open(filePath);
    ofstream << SaveToJson();
    ofstream.close();

    return SaveToJson();
  }

  std::string Transform::SaveToJson() {
    nlohmann::json json = nlohmann::json{
        {"componentType", "transform"},
        {"localPosition", {{"x", _position.x}, {"y", _position.y}, {"z", _position.z}}},
        {"localRotation",
         {{"w", _rotation.w}, {"x", _rotation.x}, {"y", _rotation.y}, {"z", _rotation.z}}},
        {"localScale", {{"x", _scale.x}, {"y", _scale.y}, {"z", _scale.z}}}};

    return json.dump(4);
  }

  auto Transform::LoadFromJson(std::string filePath) -> void {
    nlohmann::json json;
    if (filePath[0] == '{' || filePath[0] == '\n')  // HACK: Check if string is json
      json = nlohmann::json::parse(filePath.begin(), filePath.end());
    else {
      auto content = Utility::ReadTextFile(filePath);
      json         = nlohmann::json::parse(content.begin(), content.end());
    }

    Position(glm::vec3(json["localPosition"]["x"], json["localPosition"]["y"],
                       json["localPosition"]["z"]));

    Rotation(glm::quat(json["localRotation"]["w"], json["localRotation"]["x"],
                       json["localRotation"]["y"], json["localRotation"]["z"]));

    Scale(glm::vec3(json["localScale"]["x"], json["localScale"]["y"], json["localScale"]["z"]));
  }

  auto Transform::ForceUpdate() -> void {
    const auto& scene = SceneManager::GetCurrentScene();
    if (scene == nullptr) {
      return;
    }
    scene->SceneGraph()->Update(-1);
  }

  // auto Transform::LoadFromJsonString(std::string jsonString) -> void
  //{
  //  nlohmann::json json = nlohmann::json::parse(jsonString.begin(), jsonString.end());

  //  Position(glm::vec3(json["localPosition"]["x"], json["localPosition"]["y"],
  //                     json["localPosition"]["z"]));

  //  Rotation(glm::quat(json["localRotation"]["w"], json["localRotation"]["x"],
  //                     json["localRotation"]["y"], json["localRotation"]["z"]));

  //  Scale(glm::vec3(json["localScale"]["x"], json["localScale"]["y"], json["localScale"]["z"]));
  //}
}  // namespace Engine
