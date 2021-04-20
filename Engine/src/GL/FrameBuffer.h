#pragma once
#include <pch.h>

namespace Engine::GL {
  class Framebuffer {
  private:
    GLuint _handle;

  public:
    Framebuffer() noexcept;
    ~Framebuffer();
    Framebuffer(const Framebuffer&) = delete;
    auto operator=(const Framebuffer&) = delete;
    Framebuffer(Framebuffer&& other) noexcept;
    auto operator=(Framebuffer&& other) noexcept -> Framebuffer&;
    auto GetHandle() const noexcept -> GLuint;
    auto Bind(GLenum target) noexcept -> void;
    auto IsBound(GLenum target) const noexcept -> bool;
    auto IsValid() const noexcept -> bool;

  private:
    auto Release() noexcept -> void;
  };
}  // namespace Engine::GL