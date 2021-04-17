#include "pch.h"
#include "TextureAttachment.h"
#include <GL/Context.h>

namespace Engine::GL {
  TextureAttachment::TextureAttachment(GLuint width, GLuint height) noexcept
      : _dimensions(width, height) {
    auto current_bound = Context::GetBoundTexture();

    glGenTextures(1, &_handle);
    Context::BindTexture(GL_TEXTURE_2D, _handle);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    Context::BindTexture(GL_TEXTURE_2D, current_bound);
  }
  TextureAttachment::~TextureAttachment() {
    Release();
  }
  TextureAttachment::TextureAttachment(TextureAttachment&& other) noexcept
      : _handle(other._handle), _dimensions(other._dimensions) {
    other._handle = 0;
  }
  auto TextureAttachment::operator=(TextureAttachment&& other) noexcept -> TextureAttachment& {
    if (&other == this) {
      return *this;
    }
    Release();
    _handle       = other._handle;
    _dimensions   = other._dimensions;
    other._handle = 0;
    return *this;
  }
  auto TextureAttachment::GetHandle() const noexcept -> GLuint {
    return _handle;
  }
  auto TextureAttachment::IsValid() const noexcept -> bool {
    return _handle != 0;
  }
  auto TextureAttachment::Bind(GLuint slot) noexcept -> void {
    Context::BindTexture(GL_TEXTURE_2D, _handle, slot);
  }
  auto TextureAttachment::AttachToFramebuffer(GLenum attachment) -> void {
    glFramebufferTexture(GL_FRAMEBUFFER, attachment, _handle, 0);
  }
  auto TextureAttachment::Release() -> void {
    if (IsValid()) {
      glDeleteTextures(1, &_handle);
    }
  }
}  // namespace Engine::GL
