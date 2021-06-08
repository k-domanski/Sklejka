#pragma once
#include <pch.h>

namespace Editor {
  struct TransformData {
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
  };

  auto ShouldIgnore_1(int index) -> bool;
  auto GetColliderMapping_1() -> std::vector< TransformData >;
}  // namespace Editor