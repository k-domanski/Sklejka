#pragma once
#include <pch.h>

namespace Engine::GL {
  class TextureAttachment {
  private:
    GLuint _handle;
    glm::ivec2 _dimensions;

  public:
    TextureAttachment(GLuint width, GLuint height) noexcept;
    ~TextureAttachment();
    TextureAttachment(const TextureAttachment&) = delete;
    auto operator=(const TextureAttachment&) = delete;
    TextureAttachment(TextureAttachment&& other) noexcept;
    auto operator=(TextureAttachment&& other) noexcept -> TextureAttachment&;
    auto GetHandle() const noexcept -> GLuint;
    auto IsValid() const noexcept -> bool;
    auto Bind(GLuint slot) noexcept -> void;
    auto AttachToFramebuffer(GLenum attachment) -> void;

  private:
    auto Release() -> void;
  };

}  // namespace Engine::GL
