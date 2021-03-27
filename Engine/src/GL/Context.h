#pragma once
#include <pch.h>

namespace Engine::GL {
  class Context {
  public:
    static auto VertexBuffer() noexcept -> GLuint;
    static auto IndiceBuffer() noexcept -> GLuint;
    static auto VertexArray() noexcept -> GLuint;
    static auto Shader() noexcept -> GLuint;
  };
}  // namespace Engine::GL