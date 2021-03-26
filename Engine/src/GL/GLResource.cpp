#include "pch.h"
#include "GLResource.h"

namespace Engine::GL {
  GLResource::GLResource(GLuint handle) noexcept: _handle(handle) {
  }

  GLResource::GLResource(GLResource&& other) noexcept: _handle(other._handle) {
    other._handle = 0;
  }

  GLResource& GLResource::operator=(GLResource&& other) noexcept {
    if (&other == this)
      return *this;

    _handle       = other._handle;
    other._handle = 0;

    return *this;
  }

  GLuint GLResource::GetHandle() const {
    return GLuint();
  }

  void GLResource::SetHandle(GLuint handle) {
  }
}  // namespace Engine::GL
