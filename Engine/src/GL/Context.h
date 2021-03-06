#pragma once
#include <pch.h>
#include <GL/GLEnum.h>

namespace Engine::GL {
  class Shader;

  class Context {
  private:
    /* Textures */
    static inline GLuint _activeTexture{0};
    static inline GLint _maxTextureUnits{1};
    static inline std::vector< GLuint > _boundTexture;
    static inline std::vector< GLenum > _textureTarget;
    /* Framebuffers */
    static inline std::unordered_map< GLenum, GLuint > _boundFramebuffers;
    static inline GLint _maxColorAttachments{8};
    /* Other */
    static inline glm::vec4 _clearColor{0.0f};
    static inline GLbitfield _clearBitMask{0u};
    static inline bool _faceCullingEnabled = false;
    static inline glm::ivec4 _viewport;
    static inline Face _cullFace = +Face::Back;

    /* Depth Test */
    static inline bool _depthTestEnabled  = false;
    static inline bool _depthWriteEnabled = true;

    /* Stencil Test */
    static inline bool _stencilTestEnabled = false;
    static inline uint8_t _stencilMask     = 0xFF;
    static inline uint8_t _stencilRef      = 0;
    static inline StencilFunc _stencilFunc = StencilFunc::Never;
    static inline StencilOp _sfail         = StencilOp::Keep;
    static inline StencilOp _dpfail        = StencilOp::Keep;
    static inline StencilOp _dppass        = StencilOp::Keep;

    /* Blending */
    static inline bool _blendingEnabled    = false;
    static inline BlendFunc _blendFuncSrc  = BlendFunc::SrcAlpha;
    static inline BlendFunc _blendFuncDest = BlendFunc::OneMinusSrcAlpha;

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
    /* Frame Buffers */
    static auto GetMaxColorAttachments() noexcept -> GLint;
    static auto BindFramebuffer(FramebufferTarget target, GLuint handle) noexcept -> void;
    static auto IsFramebufferBound(FramebufferTarget target, GLuint handle) noexcept -> bool;
    static auto GetBoundFramebuffer(FramebufferTarget target) -> GLuint;

    /* Wrappers */
    static auto ClearBuffers() noexcept -> void;
    static auto ClearBuffers(GLbitfield mask) noexcept -> void;
    static auto FaceCulling(bool enable) noexcept -> void;
    static auto CullFace(Face face) noexcept -> void;
    static auto Viewport() noexcept -> glm::vec4;
    static auto Viewport(int x, int y, unsigned width, unsigned height) noexcept -> void;

    /* Depth Test */
    static auto DepthTest(bool enable) noexcept -> void;
    static auto DepthWrite(bool enable) noexcept -> void;

    /* Stencil Test */
    static auto StencilTest(bool enable) noexcept -> void;
    static auto StencilMask(uint8_t mask) noexcept -> void;
    static auto StencilFunction(StencilFunc func, uint8_t ref, uint8_t mask = 0xFF) -> void;
    static auto StencilOperation(StencilOp sfail, StencilOp dpfail, StencilOp dppass) -> void;

    /* Blending */
    static auto EnableBlending(bool value) -> void;
    static auto SetBlendFunction(BlendFunc source, BlendFunc destination) -> void;

    /* Helpers */
    static auto BindUniformBlocks(const std::shared_ptr< Engine::GL::Shader >& shader) -> void;
  };
}  // namespace Engine::GL