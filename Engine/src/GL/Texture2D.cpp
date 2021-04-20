#include "pch.h"
#include "Texture2D.h"
#include <GL/Context.h>

using Engine::GL::Context;
namespace Engine::GL {
  Texture2D::Texture2D(GLint width, GLint height, const GLvoid* data) noexcept
      : _minFilter(GL_LINEAR_MIPMAP_LINEAR), _magFilter(GL_LINEAR), _wrapMode(GL_REPEAT),
        _size(width, height) {
    Create(width, height, data);
  }
  Texture2D::Texture2D(Texture2D&& other) noexcept
      : _handle(other._handle), _minFilter(other._minFilter), _magFilter(other._magFilter),
        _wrapMode(other._wrapMode) {
    other._handle = 0u;
  }
  auto Texture2D::operator=(Texture2D&& other) noexcept -> Texture2D& {
    if (&other == this)
      return *this;
    Release();
    _handle       = other._handle;
    other._handle = 0u;
    return *this;
  }
  Texture2D::~Texture2D() {
    Release();
  }
  auto Texture2D::Create(GLint width, GLint height, const GLvoid* data) -> void {
    if (_handle != 0)
      return;
    auto origBoundTex = Context::GetBoundTexture();
    auto origTarget   = Context::GetTextureTarget();

    glGenTextures(1, &_handle);
    Context::BindTexture(GL_TEXTURE_2D, _handle);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, _wrapMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, _wrapMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, _minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, _magFilter);
    glGenerateMipmap(GL_TEXTURE_2D);

    Context::BindTexture(origTarget, origBoundTex);
  }
  auto Texture2D::GetHandle() const noexcept -> GLuint {
    return _handle;
  }
  auto Texture2D::Bind(GLuint slot) noexcept -> void {
    Context::BindTexture(GL_TEXTURE_2D, _handle, slot);
  }

  auto Texture2D::FilePath() const noexcept -> std::string {
    return _filePath;
  }

  auto Texture2D::FilePath(const std::string& filePath) -> std::string {
    return _filePath = filePath;
  }
  auto Texture2D::GetSize() const noexcept -> glm::ivec2 {
    return _size;
  }
  auto Texture2D::Release() noexcept -> void {
    if (_handle != 0)
      glDeleteTextures(1, &_handle);
  }
}  // namespace Engine::GL