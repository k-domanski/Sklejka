#pragma once
#include <pch.h>
#include <GL/FrameBuffer.h>
#include <GL/TextureAttachment.h>
#include <GL/Renderbuffer.h>
#include <Utility/Utility.h>

namespace Engine::GL {
  class RenderTarget {
  private:
    glm::ivec2 _dimensions;
    Framebuffer _framebuffer;
    FramebufferTarget _target{+FramebufferTarget::ReadWrite};
    std::map< GLuint, std::shared_ptr< IFramebufferAttachment > > _colorAttachments;
    std::shared_ptr< IFramebufferAttachment > _depthAttachment;
    std::shared_ptr< IFramebufferAttachment > _stencilAttachment;
    std::shared_ptr< IFramebufferAttachment > _depthStencilAttachment;
    /* Cache attachment operations */
    std::map< GLuint, std::shared_ptr< IFramebufferAttachment > > _newColorAttachments;
    std::shared_ptr< IFramebufferAttachment > _newDepthAttachment;
    std::shared_ptr< IFramebufferAttachment > _newStencilAttachment;
    std::shared_ptr< IFramebufferAttachment > _newDepthStencilAttachment;

  public:
    Engine::DataFlags flags;
    RenderTarget(GLuint width, GLuint height) noexcept;
    RenderTarget(const RenderTarget&) = delete;
    auto operator=(const RenderTarget&) -> RenderTarget& = delete;
    RenderTarget(RenderTarget&& other) noexcept;
    auto operator=(RenderTarget&& other) noexcept -> RenderTarget&;
    auto Bind(FramebufferTarget target) noexcept -> void;
    auto GetColorAttachment(GLuint index) noexcept -> std::shared_ptr< IFramebufferAttachment >;
    auto AttachColor(GLuint index,
                     const std::shared_ptr< IFramebufferAttachment >& attachment) noexcept -> void;
    auto AttachDepth(const std::shared_ptr< IFramebufferAttachment >& attachment) noexcept -> void;
    auto AttachStencil(const std::shared_ptr< IFramebufferAttachment >& attachment) noexcept
        -> void;
    auto AttachDepthStencil(const std::shared_ptr< IFramebufferAttachment >& attachment) noexcept
        -> void;

  private:
    auto SetDrawBuffers() noexcept -> void;
    auto BindSwap(FramebufferTarget target) noexcept -> GLuint;
    auto UpdateAttachments() noexcept -> void;
    auto UpdateColorAttachments() noexcept -> void;
    auto UpdateDepthAttachment() noexcept -> void;
    auto UpdateStencilAttachment() noexcept -> void;
    auto UpdateDepthStencilAttachment() noexcept -> void;
  };

}  // namespace Engine::GL
