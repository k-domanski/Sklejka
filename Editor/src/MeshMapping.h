#pragma once
#include <pch.h>
#include "Engine.h"

using namespace Engine;
using namespace Engine::Renderer;
namespace Editor {
  struct TransformData {
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
  };
  struct TreeData {
    TransformData crown;
    TransformData trunk;
  };

  auto ShouldIgnore_1(int index) -> bool;
  auto GetColliderMapping_1() -> std::vector< TransformData >;
  auto ShouldIgnore_2(int index) -> bool;
  auto IsTree_2(int index) -> bool;
  auto TreeCollider_2() -> std::vector< TreeData >;
  auto TreeColliderIndex_2(int index) -> int;
  auto GetColliderMapping_2() -> std::vector< TransformData >;
  auto GetMeshMaterial_1(int index) -> std::shared_ptr< Material >;
  auto GetMeshMaterial_2(int index) -> std::shared_ptr< Material >;
}  // namespace Editor