#pragma once
#include <pch.h>

namespace Engine::Renderer {
  struct Vertex {
    glm::vec3 position{0.0f};
    glm::vec3 normal{0.0f};
    glm::vec2 uv{0.0f};
    glm::ivec4 jointIDs{ 0 };
    glm::vec4 weights{ 0.0f };

    static auto EnableVertexLayout() -> void;
  };
}  // namespace Engine::Renderer