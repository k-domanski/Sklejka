#pragma once
#include <pch.h>

namespace Engine::GL {
  class RenderTarget;
  class Framebuffer;
  class TextureAttachment;
  class Renderbuffer;
}

namespace Engine::Renderer {
  class PingPongBuffer {
  private:
    std::shared_ptr< GL::RenderTarget > _renderTarget;
  };
}