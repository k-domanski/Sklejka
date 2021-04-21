#include "pch.h"
#include "FrameBuffer.h"
#include <GL/Context.h>

namespace Engine::GL {
  Framebuffer::Framebuffer() noexcept {
    glGenFramebuffers(1, &_handle);
  }
  Framebuffer::~Framebuffer() {
    Release();
  }
  Framebuffer::Framebuffer(Framebuffer&& other) noexcept: _handle(other._handle) {
    other._handle = 0;
  }
  auto Framebuffer::operator=(Framebuffer&& other) noexcept -> Framebuffer& {
    if (this == &other) {
      return *this;
    }
    Release();
    _handle       = other._handle;
    other._handle = 0;
    return *this;
  }
  auto Framebuffer::GetHandle() const noexcept -> GLuint {
    return _handle;
  }
  auto Framebuffer::Bind(FramebufferTarget target) noexcept -> void {
    Context::BindFramebuffer(target, _handle);
  }
  auto Framebuffer::IsBound(FramebufferTarget target) const noexcept -> bool {
    return Context::IsFramebufferBound(target, _handle);
  }
  auto Framebuffer::IsValid() const noexcept -> bool {
    return _handle != 0;
  }
  auto Framebuffer::Release() noexcept -> void {
    if (IsValid()) {
      glDeleteFramebuffers(1, &_handle);
    }
  }
}  // namespace Engine::GL
