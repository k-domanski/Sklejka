#include "pch.h"
#include "TextureBase.h"

namespace Engine::GL {
  TextureBase::TextureBase(GLenum target) noexcept
      : _bindTarget(target), _minFilter(GL_LINEAR_MIPMAP_LINEAR), _magFilter(GL_LINEAR),
        _wrapMode(GL_REPEAT) {
  }
  TextureBase::TextureBase(TextureBase&& other) noexcept
      : _handle(other._handle), _bindTarget(other._bindTarget), _minFilter(other._minFilter),
        _magFilter(other._magFilter), _wrapMode(other._wrapMode) {
    other._handle = 0u;
  }
  auto TextureBase::operator=(TextureBase&& other) noexcept -> TextureBase& {
    if (&other == this)
      return *this;
    Release();
    _handle       = other._handle;
    other._handle = 0u;
    return *this;
  }
  TextureBase::~TextureBase() {
    Release();
  }
  auto TextureBase::Create(GLint width, GLint height, const GLvoid* data) -> void {
    if (_handle != 0)
      return;
    glGenTextures(1, &_handle);
    glBindTexture(_bindTarget, _handle);
    //TODO: Change Later
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, _wrapMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, _wrapMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, _minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, _magFilter);
    glGenerateMipmap(GL_TEXTURE_2D);

  }
  auto TextureBase::Bind(GLuint index) noexcept -> void {
    glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(GL_TEXTURE_2D, _handle);
  }
  auto TextureBase::Release() noexcept -> void {
    glDeleteTextures(1, &_handle);
  }
}  // namespace Engine::GL