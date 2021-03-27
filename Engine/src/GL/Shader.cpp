#include "pch.h"
#include "Shader.h"

namespace Engine::GL {

  Shader::Shader(Shader&& other) noexcept
      : _handle(other._handle), _subShaders(std::move(other._subShaders)),
        _isLinked(other._isLinked) {
    other._handle   = 0;
    other._isLinked = false;
  }

  Shader::~Shader() {
    Release();
  }

  auto Shader::operator=(Shader&& other) noexcept -> Shader& {
    if (&other == this)
      return *this;
    Release();
    _handle         = other._handle;
    _isLinked       = other._isLinked;
    _subShaders     = std::move(other._subShaders);
    other._handle   = 0;
    other._isLinked = false;
    return *this;
  }

  auto Shader::GetHandle() const noexcept -> GLuint {
    return _handle;
  }

  auto Shader::AttachShader(const std::shared_ptr< SubShader >& shader) -> void {
    if (_handle == 0)
      _handle = glCreateProgram();

    auto it = std::find_if(_subShaders.begin(), _subShaders.end(), [&shader](const auto& item) {
      return item->GetHandle() == shader->GetHandle();
    });
    if (it == _subShaders.end())
      return;
    glAttachShader(_handle, shader->GetHandle());
  }

  auto Shader::Link() -> bool {
    if (_handle == 0) {
      _isLinked = false;
      return false;
    }
    glLinkProgram(_handle);
    if (ShaderLinkStatus() == GL_FALSE) {
      LogLinkMessage();
      _isLinked = false;
      return false;
    }
    _isLinked = true;
    return true;
  }

  auto Shader::Release() -> void {
    if (_handle != 0)
      glDeleteProgram(_handle);
  }

  auto Shader::ShaderLinkStatus() -> GLint {
    GLint param;
    glGetProgramiv(_handle, GL_LINK_STATUS, &param);
    return param;
  }

  auto Shader::LogLinkMessage() -> void {
    GLint stringLength;
    glGetProgramiv(_handle, GL_INFO_LOG_LENGTH, &stringLength);
    std::string message(stringLength, '\0');
    glGetProgramInfoLog(_handle, message.size(), NULL, &message[0]);
    // TODO: Log link message
  }

}  // namespace Engine::GL