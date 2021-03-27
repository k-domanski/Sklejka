#pragma once
#include <pch.h>

namespace Engine::GL {
  class Context {
  public:
    static auto VertexBuffer() noexcept -> GLuint;
    static auto IndiceBuffer() noexcept -> GLuint;
  };
}  // namespace Engine::GL