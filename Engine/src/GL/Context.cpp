#include "pch.h"
#include <GL/Context.h>
#include <GL/Buffer.h>
#include <GL/VertexArray.h>
#include <GL/Shader.h>
#include <GL/GLError.h>

namespace Engine::GL {
  auto Context::Initialize() noexcept -> void {
    /* Debug */
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(MessageCallback, NULL);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL,
                          GL_FALSE);

    /* Alpha Blending */
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    /* Depth Testing */
    DepthTest(true);

    /* Face Culling */
    FaceCulling(true);

    /* View Port */
    glGetIntegerv(GL_VIEWPORT, &_viewport[0]);

    /* Textures */
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &_maxTextureUnits);
    _boundTexture.resize(_maxTextureUnits, 0u);
    _textureTarget.resize(_maxTextureUnits, NULL);

    /* Framebuffers */
    glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &_maxColorAttachments);

    /* Stencil */
    glClearStencil(0x00);
    glStencilMask(_stencilMask);
  }
  auto Context::VertexBuffer() noexcept -> GLenum {
    return VertexBuffer::GetCurrentHandle();
  }
  auto Context::IndiceBuffer() noexcept -> GLenum {
    return IndiceBuffer::GetCurrentHandle();
  }
  auto Context::VertexArray() noexcept -> GLuint {
    return VertexArray::GetCurrentHandle();
  }
  auto Context::Shader() noexcept -> GLuint {
    return Engine::GL::Shader::GetCurrentHandle();
  }
  auto Context::GetClearColor() noexcept -> glm::vec4 {
    return _clearColor;
  }
  auto Context::SetClearColor(const glm::vec4& color) noexcept -> void {
    if (_clearColor == color)
      return;
    /* Apply gamma correction */
    auto g_color = glm::pow(glm::vec3(color), glm::vec3(2.2f));
    glClearColor(g_color.r, g_color.g, g_color.b, color.a);
    _clearColor = color;
  }
  auto Context::SetClearBufferMask(GLbitfield mask) noexcept -> void {
    _clearBitMask = mask;
  }
  auto Context::GetClearBufferMask() noexcept -> GLbitfield {
    return _clearBitMask;
  }
  auto Context::GetBoundTexture(GLuint slot) noexcept -> GLuint {
    if (slot >= _maxTextureUnits) {
      CORE_ERROR("Texture slot index greater than maximum allowed texture units\n[Index: {0}][Max "
                 "allowed: {1}]",
                 slot, _maxTextureUnits);
    }
    slot = glm::clamp(slot, GLuint(0), GLuint(_maxTextureUnits - 1));
    return _boundTexture[slot];
  }
  auto Context::GetBoundTexture() noexcept -> GLuint {
    return _boundTexture[_activeTexture];
  }
  auto Context::GetBoundTextureSlot(GLuint handle) noexcept -> GLint {
    auto it = std::find(_boundTexture.begin(), _boundTexture.end(), handle);
    if (it == _boundTexture.end())
      return -1;
    return GLint(it - _boundTexture.begin());
  }
  auto Context::GetTextureTarget(GLuint slot) noexcept -> GLenum {
    if (slot >= _maxTextureUnits) {
      CORE_ERROR("Texture slot index greater than maximum allowed texture units\n[Index: {0}][Max "
                 "allowed: {1}]",
                 slot, _maxTextureUnits);
    }
    slot = glm::clamp(slot, GLuint(0), GLuint(_maxTextureUnits - 1));
    return _textureTarget[slot];
  }
  auto Context::GetTextureTarget() noexcept -> GLenum {
    return _textureTarget[_activeTexture];
  }
  auto Context::SetActiveTexture(GLuint slot) noexcept -> void {
    if (slot == _activeTexture)
      return;
    if (slot >= _maxTextureUnits) {
      CORE_ERROR("Texture slot index greater than maximum allowed texture units\n[Index: {0}][Max "
                 "allowed: {1}]",
                 slot, _maxTextureUnits);
      return;
    }
    _activeTexture = slot;
    glActiveTexture(GL_TEXTURE0 + slot);
  }
  auto Context::GetActiveTexture() noexcept -> GLuint {
    return _activeTexture;
  }
  auto Context::BindTexture(GLenum target, GLuint handle, GLuint slot) noexcept -> void {
    if (GetBoundTexture(slot) == handle)
      return;
    if (slot >= _maxTextureUnits) {
      CORE_ERROR("Texture slot index greater than maximum allowed texture units\n[Index: {0}][Max "
                 "allowed: {1}]",
                 slot, _maxTextureUnits);
      return;
    }
    SetActiveTexture(slot);
    _textureTarget[slot] = target;
    glBindTexture(target, handle);
  }
  auto Context::BindTexture(GLenum target, GLuint handle) noexcept -> void {
    if (GetBoundTexture(_activeTexture) == handle)
      return;
    _textureTarget[_activeTexture] = target;
    glBindTexture(target, handle);
  }
  auto Context::GetMaxColorAttachments() noexcept -> GLint {
    return _maxColorAttachments;
  }
  auto Context::BindFramebuffer(FramebufferTarget target, GLuint handle) noexcept -> void {
    if (IsFramebufferBound(target, handle)) {
      return;
    }
    _boundFramebuffers[target] = handle;
    glBindFramebuffer(target, handle);
  }
  auto Context::IsFramebufferBound(FramebufferTarget target, GLuint handle) noexcept -> bool {
    if (_boundFramebuffers.count(target) != 0) {
      if (_boundFramebuffers[target] == handle) {
        return true;
      }
    }
    return false;
  }
  auto Context::GetBoundFramebuffer(FramebufferTarget target) -> GLuint {
    if (_boundFramebuffers.count(target) == 0) {
      return 0;
    }
    return _boundFramebuffers[target];
  }
  auto Context::ClearBuffers() noexcept -> void {
    glClear(_clearBitMask);
  }
  auto Context::ClearBuffers(GLbitfield mask) noexcept -> void {
    glClear(mask);
  }
  auto Context::DepthTest(bool enable) noexcept -> void {
    if (_depthTestEnabled == enable) {
      return;
    }
    if (enable) {
      glEnable(GL_DEPTH_TEST);
    } else {
      glDisable(GL_DEPTH_TEST);
    }
    _depthTestEnabled = enable;
  }
  auto Context::DepthWrite(bool enable) noexcept -> void {
    if (enable == _depthWriteEnabled) {
      return;
    }

    glDepthMask(enable ? GL_TRUE : GL_FALSE);

    _depthWriteEnabled = enable;
  }
  auto Context::StencilTest(bool enable) noexcept -> void {
    if (_stencilTestEnabled == enable) {
      return;
    }
    if (enable) {
      glEnable(GL_STENCIL_TEST);
    } else {
      glDisable(GL_STENCIL_TEST);
    }
    _stencilTestEnabled = enable;
  }
  auto Context::StencilMask(uint8_t mask) noexcept -> void {
    if (_stencilMask == mask) {
      return;
    }

    glStencilMask(mask);
    _stencilMask = mask;
  }
  auto Context::StencilFunction(StencilFunc func, uint8_t ref, uint8_t mask) -> void {
    if (func == _stencilFunc && ref == _stencilRef /*&& mask == _stencilMask*/) {
      return;
    }

    glStencilFunc(func, ref, mask);

    _stencilFunc = func;
    _stencilRef  = ref;
    //_stencilMask = mask;
  }
  auto Context::StencilOperation(StencilOp sfail, StencilOp dpfail, StencilOp dppass) -> void {
    if (_sfail == sfail && _dpfail == dpfail && _dppass == dppass) {
      return;
    }

    glStencilOp(sfail, dpfail, dppass);

    _sfail  = sfail;
    _dpfail = dpfail;
    _dppass = dppass;
  }
  auto Context::FaceCulling(bool enable) noexcept -> void {
    if (_faceCullingEnabled == enable) {
      return;
    }
    if (enable) {
      glEnable(GL_CULL_FACE);
    } else {
      glDisable(GL_CULL_FACE);
    }
    _faceCullingEnabled = enable;
  }
  auto Context::CullFace(Face face) noexcept -> void {
    if (face == _cullFace) {
      return;
    }
    glCullFace(face);
    _cullFace = face;
  }
  auto Context::Viewport() noexcept -> glm::vec4 {
    return _viewport;
  }
  auto Context::Viewport(int x, int y, unsigned width, unsigned height) noexcept -> void {
    if (_viewport == glm::ivec4{x, y, width, height}) {
      return;
    }
    glViewport(x, y, width, height);
    _viewport = {x, y, width, height};
  }
  auto Context::EnableBlending(bool value) -> void {
    if (value == _blendingEnabled) {
      return;
    }
    if (value) {
      glEnable(GL_BLEND);
    } else {
      glDisable(GL_BLEND);
    }
    _blendingEnabled = value;
  }
  auto Context::SetBlendFunction(BlendFunc source, BlendFunc destination) -> void {
    if (source == _blendFuncSrc && destination == _blendFuncDest) {
      return;
    }

    _blendFuncSrc  = source;
    _blendFuncDest = destination;
    glBlendFunc(_blendFuncSrc, _blendFuncDest);
  }
  auto Context::BindUniformBlocks(const std::shared_ptr< Engine::GL::Shader >& shader) -> void {
    shader->BindUniformBlock("u_Transform", UniformBlock::TransformData);
    shader->BindUniformBlock("u_Camera", UniformBlock::CameraData);
    shader->BindUniformBlock("u_DirectionalLight", UniformBlock::LightData);
    shader->BindUniformBlock("u_ShadowData", UniformBlock::ShadowData);
    shader->BindUniformBlock("u_JointData", UniformBlock::JointData);
  }
}  // namespace Engine::GL
