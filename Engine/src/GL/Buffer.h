#pragma once
#include <GL/GLResource.h>
#include <GL/IGLResource.h>

namespace Engine::GL {
  template< GLenum BufferType >
  class Buffer : public IGLResource {
  private:
    static GLuint s_currentHandle;
    GLuint _handle;

  public:
    static GLuint GetCurrentHandle() noexcept {
      return s_currentHandle;
    }
    Buffer() noexcept {
      glGenBuffers(1, &_handle);
    }
    Buffer(Buffer< BufferType >&& other) noexcept: _handle(other._handle) {
      other._handle = 0u;
    }
    Buffer< BufferType >& operator=(Buffer< BufferType >&& other) noexcept {
      if (&other == *this)
        return *this;
      Release();
      _handle       = other._handle;
      other._handle = 0u;
      return *this;
    }
    GLuint GetHandle() const override {
      return _handle;
    }
    void Bind() noexcept {
      if (IsBound())
        return;
      glBindBuffer(BufferType, _handle);
      s_currentHandle = _handle;
    }
    bool IsBound() const noexcept {
      return _handle == s_currentHandle;
    }
    bool IsValid() const noexcept {
      return _handle != 0u;
    }
    void SetData(GLsizeiptr size, const void* data, GLenum usage = GL_STATIC_DRAW) noexcept {
      Bind();
      glBufferData(BufferType, size, data, usage);
    }
    constexpr GLenum GetBufferType() const noexcept {
      return BufferType;
    }

  private:
    void Release() noexcept {
      glDeleteBuffers(1, &_handle);
      _handle = 0;
    }
  };

  template< GLenum BufferType >
  GLuint Buffer< BufferType >::s_currentHandle = 0u;

  /* GL Buffers */
  typedef Buffer< GL_ARRAY_BUFFER > VertexBuffer;
  typedef Buffer< GL_ELEMENT_ARRAY_BUFFER > IndexBuffer;
}  // namespace Engine::GL