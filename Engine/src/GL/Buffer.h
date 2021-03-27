#pragma once
#include <GL/GLResource.h>
#include <GL/IGLResource.h>

namespace Engine::GL {
  template< GLenum BufferType >
  class Buffer : public IGLResource {
  private:
    static inline GLuint s_currentHandle = 0u;
    GLuint _handle;

  public:
    static auto GetCurrentHandle() noexcept -> GLuint {
      return s_currentHandle;
    }
    Buffer() noexcept {
      glGenBuffers(1, &_handle);
    }
    Buffer(Buffer< BufferType >&& other) noexcept: _handle(other._handle) {
      other._handle = 0u;
    }
    auto operator=(Buffer< BufferType >&& other) noexcept -> Buffer< BufferType >& {
      if (&other == this)
        return *this;
      Release();
      _handle       = other._handle;
      other._handle = 0u;
      return *this;
    }
    auto GetHandle() const noexcept -> GLuint override {
      return _handle;
    }
    auto Bind() noexcept -> void {
      if (IsBound())
        return;
      glBindBuffer(BufferType, _handle);
      s_currentHandle = _handle;
    }
    auto IsBound() const noexcept -> bool {
      return _handle == s_currentHandle;
    }
    auto IsValid() const noexcept -> bool {
      return _handle != 0u;
    }
    auto SetData(GLsizeiptr size, const void* data, GLenum usage = GL_STATIC_DRAW) noexcept
        -> void {
      if (!IsValid())
        return;
      Bind();
      glBufferData(BufferType, size, data, usage);
    }
    constexpr auto GetBufferType() const noexcept -> GLenum {
      return BufferType;
    }

  private:
    auto Release() noexcept -> void {
      glDeleteBuffers(1, &_handle);
      _handle = 0;
    }
  };

  /* GL Buffers */
  typedef Buffer< GL_ARRAY_BUFFER > VertexBuffer;
  typedef Buffer< GL_ELEMENT_ARRAY_BUFFER > IndiceBuffer;
}  // namespace Engine::GL