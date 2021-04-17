#pragma once
#include <pch.h>

namespace Engine::GL {
  class Renderbuffer {
  private:
    GLuint _handle;
    glm::ivec2 _dimensions;

  public:
    Renderbuffer(GLuint width, GLuint height) noexcept;
    ~Renderbuffer();
    Renderbuffer(const Renderbuffer&) = delete;
    auto operator=(const Renderbuffer&) = delete;
    Renderbuffer(Renderbuffer&& other) noexcept;
    auto operator=(Renderbuffer&& other) noexcept -> Renderbuffer&;
    auto IsValid() const noexcept -> bool;
    auto AttachToFramebuffer(GLenum attachment) -> void;

  private:
    auto Release() noexcept -> void;
  };
}  // namespace Engine::GL