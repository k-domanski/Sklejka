#pragma once
#include <pch.h>

namespace Engine::GL {
  struct CameraUniformData {
    glm::mat4 view;       // 16 byte alignment
    glm::mat4 projection; // 16 byte alignment
  };
}  // namespace Engine::GL