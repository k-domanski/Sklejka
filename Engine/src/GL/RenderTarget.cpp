#include "pch.h"
#include "RenderTarget.h"
#include <GL/Context.h>

namespace Engine::GL {
  RenderTarget::RenderTarget(GLuint width, GLuint height) noexcept
      : _dimensions(width, height), _target(GL_FRAMEBUFFER), _depthAttachment(nullptr),
        _stencilAttachment(nullptr), _depthStencilAttachment(nullptr) {
  }
  RenderTarget::RenderTarget(RenderTarget&& other) noexcept
      : _dimensions(other._dimensions), _framebuffer(std::move(other._framebuffer)),
        _target(other._target), _colorAttachments(std::move(other._colorAttachments)),
        _depthAttachment(std::move(other._depthAttachment)),
        _stencilAttachment(std::move(other._stencilAttachment)),
        _depthStencilAttachment(std::move(other._depthStencilAttachment)) {
  }
  auto RenderTarget::operator=(RenderTarget&& other) noexcept -> RenderTarget& {
    if (&other == this) {
      return *this;
    }
    _dimensions             = other._dimensions;
    _framebuffer            = std::move(other._framebuffer);
    _colorAttachments       = std::move(other._colorAttachments);
    _depthAttachment        = std::move(other._depthAttachment);
    _stencilAttachment      = std::move(other._stencilAttachment);
    _depthStencilAttachment = std::move(other._depthStencilAttachment);
    _target                 = other._target;
    return *this;
  }
  auto RenderTarget::Bind(GLenum target) noexcept -> void {
    _target = target;
    _framebuffer.Bind(target);
    // Can be set once per state change
    SetDrawBuffers();
  }
  auto RenderTarget::GetColorAttachment(GLuint index) noexcept
      -> std::shared_ptr< IFramebufferAttachment > {
    if (_colorAttachments.count(index) == 0) {
      return nullptr;
    }
    return _colorAttachments[index];
  }
  auto RenderTarget::AttachColor(
      GLuint index, const std::shared_ptr< IFramebufferAttachment >& attachment) noexcept -> void {
    if (index >= Context::GetMaxColorAttachments()) {
      LOG_WARN("Color attachment index not available: Index [{}], Max [{}]", index,
               Context::GetMaxColorAttachments());
      return;
    }
    auto current_framebuffer = BindSwap(_target);
    if (attachment == nullptr && _colorAttachments.count(index) != 0) {
      _colorAttachments[index]->DetachFromFramebuffer(_target, GL_COLOR_ATTACHMENT0 + index);
      _colorAttachments.erase(index);
      Context::BindFramebuffer(_target, current_framebuffer);
      return;
    }
    if (_colorAttachments.count(index) != 0 && _colorAttachments[index] == attachment) {
      Context::BindFramebuffer(_target, current_framebuffer);
      return;
    }
    attachment->AttachToFramebuffer(_target, GL_COLOR_ATTACHMENT0 + index);
    _colorAttachments[index] = attachment;
    Context::BindFramebuffer(_target, current_framebuffer);
  }
  auto RenderTarget::AttachDepth(
      const std::shared_ptr< IFramebufferAttachment >& attachment) noexcept -> void {
    if (_depthAttachment == attachment) {
      return;
    }
    auto current_framebuffer = BindSwap(_target);
    // Detach combined if attached
    if (attachment == nullptr) {
      _depthAttachment->DetachFromFramebuffer(_target, GL_DEPTH_ATTACHMENT);
    } else {
      if (_depthStencilAttachment != nullptr) {
        _depthStencilAttachment->DetachFromFramebuffer(_target, GL_DEPTH_STENCIL_ATTACHMENT);
        _depthStencilAttachment = nullptr;
      }
      attachment->AttachToFramebuffer(_target, GL_DEPTH_ATTACHMENT);
    }
    _depthAttachment = attachment;
    Context::BindFramebuffer(_target, current_framebuffer);
  }
  auto RenderTarget::AttachStencil(
      const std::shared_ptr< IFramebufferAttachment >& attachment) noexcept -> void {
    if (_stencilAttachment == attachment) {
      return;
    }
    auto current_framebuffer = BindSwap(_target);
    // Detach combined if attached
    if (attachment == nullptr) {
      _stencilAttachment->DetachFromFramebuffer(_target, GL_STENCIL_ATTACHMENT);
    } else {
      if (_depthStencilAttachment != nullptr) {
        _depthStencilAttachment->DetachFromFramebuffer(_target, GL_DEPTH_STENCIL_ATTACHMENT);
        _depthStencilAttachment = nullptr;
      }
      attachment->AttachToFramebuffer(_target, GL_STENCIL_ATTACHMENT);
    }
    _stencilAttachment = attachment;
    Context::BindFramebuffer(_target, current_framebuffer);
  }
  auto RenderTarget::AttachDepthStencil(
      const std::shared_ptr< IFramebufferAttachment >& attachment) noexcept -> void {
    if (_depthStencilAttachment == attachment) {
      return;
    }
    auto current_framebuffer = BindSwap(_target);
    // Detach combined if attached
    if (attachment == nullptr) {
      _depthStencilAttachment->DetachFromFramebuffer(_target, GL_DEPTH_STENCIL_ATTACHMENT);
    } else {
      if (_depthAttachment != nullptr) {
        _depthAttachment->DetachFromFramebuffer(_target, GL_DEPTH_ATTACHMENT);
        _depthAttachment = nullptr;
      }
      if (_stencilAttachment != nullptr) {
        _stencilAttachment->DetachFromFramebuffer(_target, GL_STENCIL_ATTACHMENT);
        _stencilAttachment = nullptr;
      }
      attachment->AttachToFramebuffer(_target, GL_DEPTH_STENCIL_ATTACHMENT);
    }
    _depthStencilAttachment = attachment;
    Context::BindFramebuffer(_target, current_framebuffer);
  }
  auto RenderTarget::SetDrawBuffers() noexcept -> void {
    std::vector< GLenum > buffers;
    buffers.reserve(_colorAttachments.size());
    for (const auto& [index, _] : _colorAttachments) {
      buffers.push_back(GL_COLOR_ATTACHMENT0 + index);
    }
    glDrawBuffers(buffers.size(), buffers.data());
  }
  auto RenderTarget::BindSwap(GLenum target) noexcept -> GLuint {
    auto bound_framebuffer = Context::GetBoundFramebuffer(_target);
    _framebuffer.Bind(_target);
    return bound_framebuffer;
  }
};  // namespace Engine::GL