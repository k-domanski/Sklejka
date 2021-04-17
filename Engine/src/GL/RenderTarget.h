#pragma once
#include <pch.h>
#include <GL/FrameBuffer.h>
#include <GL/TextureAttachment.h>
#include <GL/Renderbuffer.h>
#include <GL/Context.h>

namespace Engine::GL {
  template< typename CBufferType, typename DSBufferType >
  class RenderTarget {
  private:
    glm::ivec2 _dimensions;
    Framebuffer _framebuffer;
    CBufferType _cbuffer;
    DSBufferType _dsbuffer;
    GLenum _target = GL_FRAMEBUFFER;

  public:
    RenderTarget(GLuint width, GLuint height) noexcept
        : _dimensions(widht, height), _cbuffer(widht, height), _dsbuffer(width, height) {
      auto bound_framebuffer = Context::GetBoundFramebuffer(_target);
      _framebuffer.Bind(_target);
      _cbuffer.AttachToFramebuffer(GL_COLOR_ATTACHMENT0);
      _dsbuffer.AttachToFramebuffer(GL_DEPTH_STENCIL_ATTACHMENT);
      Context::BindFramebuffer(_target, bound_framebuffer);
    }
    RenderTarget(const RenderTarget&) = delete;
    auto operator=(const RenderTarget&) = delete;
    RenderTarget(RenderTarget&& other) noexcept
        : _dimensions(other._dimensions), _framebuffer(std::move(other._framebuffer)),
          _cbuffer(std::move(other._cbuffer)), _dsbuffer(std::move(other._dsbuffer)),
          _target(other._target) {
    }
    auto operator=(RenderTarget&& other) noexcept -> RenderTarget< CBufferType, DSBufferType >& {
      if (&other == this) {
        return *this;
      }
      _dimensions  = other._dimensions;
      _framebuffer = std::move(other._framebuffer);
      _cbuffer     = std::move(other._cbuffer);
      _dsbuffer    = std::move(other._dsbuffer);
      return *this;
    }
    auto Bind() noexcept -> void {
      _framebuffer.Bind(_target);
    }
  };

  typedef RenderTarget< TextureAttachment, Renderbuffer > RenderTextureTarget;
}  // namespace Engine::GL
