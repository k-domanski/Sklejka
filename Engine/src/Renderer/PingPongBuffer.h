#pragma once
#include <pch.h>
#include <GL/GLEnum.h>

namespace Engine::GL {
  class RenderTarget;
  class Framebuffer;
  class TextureAttachment;
  class Renderbuffer;
}  // namespace Engine::GL

namespace Engine::Renderer {
  class PingPongBuffer {
  private:
    std::shared_ptr< GL::RenderTarget > _renderTargets[2];
    std::shared_ptr< GL::TextureAttachment > _renderTextures[2];
    glm::vec2 _size;
    unsigned _fb[2]{0, 1};
    GL::FramebufferTarget _target{GL::FramebufferTarget::ReadWrite};

  public:
    PingPongBuffer(glm::vec2 size);
    auto Swap() noexcept -> void;
    auto Bind(GL::FramebufferTarget target) noexcept -> void;
    auto Front() const noexcept -> std::shared_ptr< GL::RenderTarget >;
    auto Front(const std::shared_ptr< GL::RenderTarget >& renderTarget) noexcept
        -> std::shared_ptr< GL::RenderTarget >;
    auto Back() const noexcept -> std::shared_ptr< GL::RenderTarget >;
    auto Back(const std::shared_ptr< GL::RenderTarget >& renderTarget) noexcept
        -> std::shared_ptr< GL::RenderTarget >;
    auto FrontTexture() const noexcept -> std::shared_ptr< GL::TextureAttachment >;
    auto BackTexture() const noexcept -> std::shared_ptr< GL::TextureAttachment >;
  };
}  // namespace Engine::Renderer