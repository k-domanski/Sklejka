#pragma once
#include <pch.h>

namespace Engine::GL {
  class Texture2D {
  private:
    GLuint _handle = 0u;
    GLint _minFilter;
    GLint _magFilter;
    GLint _wrapMode;

  public:
    Texture2D(GLint width, GLint height, const GLvoid* data) noexcept;
    Texture2D(const Texture2D&) = delete;
    auto operator=(const Texture2D&) -> Texture2D& = delete;
    Texture2D(Texture2D&& other) noexcept;
    auto operator=(Texture2D&& other) noexcept -> Texture2D&;
    ~Texture2D();
    auto Bind(GLuint index) noexcept -> void;

  private:
    auto Create(GLint width, GLint height, const GLvoid* data) -> void;
    auto Release() noexcept -> void;
  };
}  // namespace Engine::GL