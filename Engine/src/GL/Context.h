#pragma once
#include <pch.h>

namespace Engine::GL {
  class Context {
  private:
    static inline glm::vec4 _clearColor{0.0f};
    static inline GLbitfield _clearBitMask{0u};
    /* Textures */
    static inline GLuint _activeTexture{0};
    static inline GLint _maxTextureUnits{1};
    static inline std::vector< GLuint > _boundTexture;
    static inline std::vector< GLenum > _textureTarget;

  public:
    static auto Initialize() noexcept -> void;
    /* Get / Set */
    static auto GetClearColor() noexcept -> glm::vec4;
    static auto SetClearColor(const glm::vec4& color) noexcept -> void;
    static auto SetClearBufferMask(GLbitfield mask) noexcept -> void;
    static auto GetClearBufferMask() noexcept -> GLbitfield;

    static auto VertexBuffer() noexcept -> GLuint;
    static auto IndiceBuffer() noexcept -> GLuint;
    static auto VertexArray() noexcept -> GLuint;
    static auto Shader() noexcept -> GLuint;
    /* Textures */
    static auto GetBoundTexture(GLuint slot) noexcept -> GLuint;
    static auto GetBoundTexture() noexcept -> GLuint;
    static auto GetBoundTextureSlot(GLuint handle) noexcept -> GLint;
    static auto GetTextureTarget(GLuint slot) noexcept -> GLenum;
    static auto GetTextureTarget() noexcept -> GLenum;
    static auto SetActiveTexture(GLuint slot) noexcept -> void;
    static auto GetActiveTexture() noexcept -> GLuint;
    static auto BindTexture(GLenum target, GLuint handle, GLuint slot) noexcept -> void;
    static auto BindTexture(GLenum target, GLuint handle) noexcept -> void;
    
    /* Wrappers */
    static auto ClearBuffers() noexcept -> void;
    static auto ClearBuffers(GLbitfield mask) noexcept -> void;
  };
}  // namespace Engine::GL