#pragma once
#include <pch.h>

namespace Engine::GL {
  class GLResource {
  private:
    GLuint _handle = 0;

  public:
    GLResource() noexcept = default;
    GLResource(GLuint handle) noexcept;
    GLResource(GLResource&& other) noexcept;
    GLResource& operator=(GLResource&& other) noexcept;

    GLResource(const GLResource& other) = delete;
    GLResource& operator=(const GLResource& other) = delete;

    GLuint GetHandle() const;
    void SetHandle(GLuint handle);
  };
}  // namespace Engine::GL