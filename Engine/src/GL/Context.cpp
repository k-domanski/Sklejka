#include "pch.h"
#include <GL/Context.h>
#include <GL/Buffer.h>
#include <GL/VertexArray.h>
#include <GL/Shader.h>
#include <GL/GLError.h>

namespace Engine::GL {
  auto Context::Initialize() noexcept -> void {
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(MessageCallback, NULL);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    /*glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL,
                          GL_FALSE);*/
  }
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
  auto Context::GetClearColor() noexcept -> glm::vec4 {
    return _clearColor;
  }
  auto Context::SetClearColor(const glm::vec4& color) noexcept -> void {
    if (_clearColor == color)
      return;
    glClearColor(color.r, color.g, color.b, color.a);
    _clearColor = color;
  }
  auto Context::SetClearBufferMask(GLbitfield mask) noexcept -> void {
    _clearBitMask = mask;
  }
  auto Context::GetClearBufferMask() noexcept -> GLbitfield {
    return _clearBitMask;
  }
  auto Context::ClearBuffers() noexcept -> void {
    glClear(_clearBitMask);
  }
  auto Context::ClearBuffers(GLbitfield mask) noexcept -> void {
    glClear(mask);
  }
}  // namespace Engine::GL
