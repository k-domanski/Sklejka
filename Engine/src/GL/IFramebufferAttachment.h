#pragma once
#include <pch.h>

namespace Engine::GL {
  BETTER_ENUM(__AttachmentType, uint8_t, Texture = 0, Renderbuffer = 1);
  typedef __AttachmentType AttachmentType;

  class IFramebufferAttachment {
  public:
    virtual ~IFramebufferAttachment()                                                     = default;
    virtual auto AttachToFramebuffer(GLenum target, GLenum attachment) noexcept -> void   = 0;
    virtual auto DetachFromFramebuffer(GLenum target, GLenum attachment) noexcept -> void = 0;
    virtual auto Type() const noexcept -> AttachmentType                                  = 0;
  };
}  // namespace Engine::GL
