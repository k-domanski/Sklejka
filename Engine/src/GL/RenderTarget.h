#pragma once
#include <pch.h>
#include <GL/FrameBuffer.h>
#include <GL/Context.h>
#include <GL/IFramebufferAttachment.h>

namespace Engine::GL {
  class RenderTarget {
  private:
    glm::ivec2 _dimensions;
    Framebuffer _framebuffer;
    GLenum _target = GL_FRAMEBUFFER;
    std::map< GLenum, std::shared_ptr< IFramebufferAttachment > > _colorAttachments;
    std::shared_ptr< IFramebufferAttachment > _depthAttachment;
    std::shared_ptr< IFramebufferAttachment > _stencilAttachment;
    std::shared_ptr< IFramebufferAttachment > _depthStencilAttachment;

  public:
    RenderTarget(GLuint width, GLuint height) noexcept;
    RenderTarget(const RenderTarget&) = delete;
    auto operator=(const RenderTarget&) = delete;
    RenderTarget(RenderTarget&& other) noexcept;
    auto operator=(RenderTarget&& other) noexcept -> RenderTarget&;
    auto Bind(GLenum target) noexcept -> void;
    auto AttachColor(GLuint index,
                     const std::shared_ptr< IFramebufferAttachment >& attachment) noexcept -> void;
    auto AttachDepth(const std::shared_ptr< IFramebufferAttachment >& attachment) noexcept -> void;
    auto AttachStencil(const std::shared_ptr< IFramebufferAttachment >& attachment) noexcept
        -> void;
    auto AttachDepthStencil(const std::shared_ptr< IFramebufferAttachment >& attachment) noexcept
        -> void;

  private:
    auto SetDrawBuffers() noexcept -> void;
    auto BindSwap(GLenum target) noexcept -> GLuint;
  };

}  // namespace Engine::GL
