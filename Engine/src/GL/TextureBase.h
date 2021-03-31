#pragma once
#include <pch.h>

namespace Engine::GL {
  class TextureBase {
  private:
    GLuint _handle = 0u;
    GLenum _bindTarget;
    GLint _minFilter;
    GLint _magFilter;
    GLint _wrapMode;

  public:
    TextureBase(GLenum target) noexcept;
    TextureBase(const TextureBase&) = delete;
    auto operator=(const TextureBase&) -> TextureBase& = delete;
    TextureBase(TextureBase&& other) noexcept;
    auto operator=(TextureBase&& other) noexcept -> TextureBase&;
    virtual ~TextureBase();
    virtual auto Create(GLint width, GLint height, const GLvoid* data) -> void;
    auto Bind(GLuint index) noexcept -> void;

  private:
    auto Release() noexcept -> void;
  };
}  // namespace Engine::GL