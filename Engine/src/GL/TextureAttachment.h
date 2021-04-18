#pragma once
#include <pch.h>
#include <GL/IFramebufferAttachment.h>

namespace Engine::GL {
  class TextureAttachment : public IFramebufferAttachment {
  private:
    GLuint _handle;
    glm::ivec2 _dimensions;
    GLint _internalFormat;
    GLenum _externalFormat;
    GLenum _type;

  public:
    TextureAttachment(GLuint width, GLuint height, GLint internalFormat, GLenum externalFormat,
                      GLenum type) noexcept;
    ~TextureAttachment();
    TextureAttachment(const TextureAttachment&) = delete;
    auto operator=(const TextureAttachment&) = delete;
    TextureAttachment(TextureAttachment&& other) noexcept;
    auto operator=(TextureAttachment&& other) noexcept -> TextureAttachment&;
    auto GetHandle() const noexcept -> GLuint;
    auto IsValid() const noexcept -> bool;
    auto Bind(GLuint slot) noexcept -> void;
    auto AttachToFramebuffer(GLenum target, GLenum attachment) noexcept -> void override;
    auto DetachFromFramebuffer(GLenum target, GLenum attachment) noexcept -> void override;
    auto Type() const noexcept -> AttachmentType override;

  private:
    auto Release() -> void;
  };

}  // namespace Engine::GL
