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
    /*glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL,
                          GL_FALSE);*/

    /* Alpha Blending */
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    /* Textures */
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &_maxTextureUnits);
    _boundTexture.resize(_maxTextureUnits, 0u);
    _textureTarget.resize(_maxTextureUnits, NULL);
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
    return Shader::GetCurrentHandle();
  }
  auto Context::GetClearColor() noexcept -> glm::vec4 {
    return _clearColor;
  }
  auto Context::SetClearColor(const glm::vec4& color) noexcept -> void {
    if (_clearColor == color)
      return;
    glClearColor(color.r, color.g, color.b, color.a);
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
  auto Context::ClearBuffers() noexcept -> void {
    glClear(_clearBitMask);
  }
  auto Context::ClearBuffers(GLbitfield mask) noexcept -> void {
    glClear(mask);
  }
}  // namespace Engine::GL
