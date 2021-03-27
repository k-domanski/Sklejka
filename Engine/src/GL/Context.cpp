#include "pch.h"
#include <GL/Context.h>
#include <GL/Buffer.h>
#include <GL/VertexArray.h>
#include <GL/Shader.h>

namespace Engine::GL {
  auto Context::VertexBuffer() noexcept -> GLenum {
    return VertexBuffer::GetCurrentHandle();
  }
  auto Context::IndiceBuffer() noexcept -> GLenum {
    return IndiceBuffer::GetCurrentHandle();
  }
  auto Context::VertexArray() noexcept -> GLuint {
    return VertexArray::GetCurrentHandle();
  }
  auto Context::Shader() noexcept -> GLuint {
    return Shader::GetCurrentHandle();
  }
}  // namespace Engine::GL
