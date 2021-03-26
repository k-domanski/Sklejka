#pragma once
#include <pch.h>

namespace Engine::GL {
  class Context {
  public:
    static GLuint VertexBuffer() noexcept;
    static GLuint IndexBuffer() noexcept;
  };
}  // namespace Engine::GL