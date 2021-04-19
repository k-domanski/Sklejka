#include "pch.h"
#include "Renderbuffer.h"

namespace Engine::GL {
  Renderbuffer::Renderbuffer(GLuint width, GLuint height) noexcept: _dimensions(width, height) {
    glGenRenderbuffers(1, &_handle);
    glBindRenderbuffer(GL_RENDERBUFFER, _handle);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
  }
  Renderbuffer::~Renderbuffer() {
    Release();
  }
  Renderbuffer::Renderbuffer(Renderbuffer&& other) noexcept
      : _handle(other._handle), _dimensions(other._dimensions) {
    other._handle = 0;
  }
  auto Renderbuffer::operator=(Renderbuffer&& other) noexcept -> Renderbuffer& {
    if (&other == this) {
      return *this;
    }
    Release();
    _handle       = other._handle;
    _dimensions   = other._dimensions;
    other._handle = 0;
    return *this;
  }
  auto Renderbuffer::IsValid() const noexcept -> bool {
    return _handle != 0;
  }
  auto Renderbuffer::AttachToFramebuffer(GLenum target, GLenum attachment) noexcept -> void {
    glFramebufferRenderbuffer(target, attachment, GL_RENDERBUFFER, _handle);
  }
  auto Renderbuffer::DetachFromFramebuffer(GLenum target, GLenum attachment) noexcept -> void {
    glFramebufferRenderbuffer(target, attachment, GL_RENDERBUFFER, 0);
  }
  auto Renderbuffer::Type() const noexcept -> AttachmentType {
    return AttachmentType::Renderbuffer;
  }
  auto Renderbuffer::Release() noexcept -> void {
    if (IsValid()) {
      glDeleteRenderbuffers(1, &_handle);
    }
  }
}  // namespace Engine::GL
