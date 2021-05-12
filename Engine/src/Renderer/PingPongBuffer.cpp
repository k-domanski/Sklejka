#include "pch.h"
#include "PingPongBuffer.h"
#include <GL/RenderTarget.h>

namespace Engine::Renderer {
  PingPongBuffer::PingPongBuffer(glm::vec2 size): _size(size) {
    _renderTargets[0]  = std::make_shared< GL::RenderTarget >(size.x, size.y);
    _renderTargets[1]  = std::make_shared< GL::RenderTarget >(size.x, size.y);
    _renderTextures[0] = std::make_shared< GL::TextureAttachment >(size.x, size.y, GL_RGB16F, GL_RGBA,
                                                                   GL_FLOAT);
    _renderTextures[1] = std::make_shared< GL::TextureAttachment >(size.x, size.y, GL_RGB16F, GL_RGBA,
                                                                   GL_FLOAT);
    _renderTargets[0]->AttachColor(0, _renderTextures[0]);
    _renderTargets[1]->AttachColor(0, _renderTextures[1]);

    auto depthStencil = std::make_shared< GL::Renderbuffer >(size.x, size.y);
    _renderTargets[0]->AttachDepthStencil(depthStencil);
    _renderTargets[1]->AttachDepthStencil(depthStencil);

    _renderTargets[0]->Bind(_target);
  }
  auto PingPongBuffer::Swap() noexcept -> void {
    std::swap(_fb[0], _fb[1]);
    Front()->Bind(_target);
  }
  auto PingPongBuffer::Bind(GL::FramebufferTarget target) noexcept -> void {
    Front()->Bind(target);
    _target = target;
  }
  auto PingPongBuffer::Front() const noexcept -> std::shared_ptr< GL::RenderTarget > {
    return _renderTargets[_fb[0]];
  }
  auto PingPongBuffer::Front(const std::shared_ptr< GL::RenderTarget >& renderTarget) noexcept
      -> std::shared_ptr< GL::RenderTarget > {
    return _renderTargets[_fb[0]] = renderTarget;
  }
  auto PingPongBuffer::Back() const noexcept -> std::shared_ptr< GL::RenderTarget > {
    return _renderTargets[_fb[1]];
  }
  auto PingPongBuffer::Back(const std::shared_ptr< GL::RenderTarget >& renderTarget) noexcept
      -> std::shared_ptr< GL::RenderTarget > {
    return _renderTargets[_fb[1]] = renderTarget;
  }
  auto PingPongBuffer::FrontTexture() const noexcept -> std::shared_ptr< GL::TextureAttachment > {
    return _renderTextures[_fb[0]];
  }
  auto PingPongBuffer::BackTexture() const noexcept -> std::shared_ptr< GL::TextureAttachment > {
    return _renderTextures[_fb[1]];
  }
}  // namespace Engine::Renderer
