#include "pch.h"
#include <GL/Context.h>
#include <GL/Buffer.h>

namespace Engine::GL {
  GLuint Context::VertexBuffer() noexcept {
    return VertexBuffer::GetCurrentHandle();
  }
  GLuint Context::IndexBuffer() noexcept {
    return IndexBuffer::GetCurrentHandle();
  }
}  // namespace Engine::GL
