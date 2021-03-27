#pragma once
#include <GL/IGLResource.h>

namespace Engine::GL {
  class VertexArray : public IGLResource {
  private:
    static inline GLuint s_currentHandle = 0u;
    GLuint _handle;

  public:
    static auto GetCurrentHandle() noexcept -> GLuint;
    VertexArray() noexcept;
    VertexArray(const VertexArray&) = delete;
    auto operator=(const VertexArray&) = delete;
    VertexArray(VertexArray&& other) noexcept;
    auto operator=(VertexArray&& other) noexcept -> VertexArray&;
    auto Bind() noexcept -> void;
    auto IsBound() const noexcept -> bool;
    auto IsValid() const noexcept -> bool;
    auto GetHandle() const noexcept -> GLuint;

  private:
    auto Release() -> void;
  };
}  // namespace Engine::GL