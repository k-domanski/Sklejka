#pragma once
#include <pch.h>

namespace Engine::GL {
  class Context {
  private:
    static inline glm::vec4 _clearColor{0.0f};
    static inline GLbitfield _clearBitMask{0u};
  public:
    static auto Initialize() noexcept -> void;
    /* Get / Set */
    static auto VertexBuffer() noexcept -> GLuint;
    static auto IndiceBuffer() noexcept -> GLuint;
    static auto VertexArray() noexcept -> GLuint;
    static auto Shader() noexcept -> GLuint;
    static auto GetClearColor() noexcept -> glm::vec4;
    static auto SetClearColor(const glm::vec4& color) noexcept -> void;
    static auto SetClearBufferMask(GLbitfield mask) noexcept -> void;
    static auto GetClearBufferMask() noexcept -> GLbitfield;
    /* Wrappers */
    static auto ClearBuffers() noexcept -> void;
    static auto ClearBuffers(GLbitfield mask) noexcept -> void;

  };
}  // namespace Engine::GL