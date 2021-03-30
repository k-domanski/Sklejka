#include "pch.h"
#include "SubShader.h"
#include <App/Log.h>

namespace Engine::GL {
  SubShader::SubShader(ShaderType type, std::string source) noexcept
      : _source(std::move(source)), _type(type) {
    _handle = glCreateShader(_type);
    Compile();
  }
  SubShader::SubShader(SubShader&& other) noexcept
      : _handle(other._handle), _source(std::move(other._source)), _isCompiled(other._isCompiled),
        _type(other._type) {
    other._handle     = 0;
    other._isCompiled = false;
  }
  auto SubShader::operator=(SubShader&& other) noexcept -> SubShader& {
    if (&other == this)
      return *this;
    Release();
    _handle           = other._handle;
    _type             = other._type;
    _source           = std::move(other._source);
    _isCompiled       = other._isCompiled;
    other._handle     = 0;
    other._isCompiled = false;
    return *this;
  }
  SubShader::~SubShader() {
    Release();
  }
  auto SubShader::GetSource() const -> const std::string& {
    return _source;
  }
  auto SubShader::GetType() const -> ShaderType {
    return _type;
  }
  auto SubShader::GetHandle() const noexcept -> GLuint {
    return _handle;
  }
  auto SubShader::Compile() -> bool {
    if (_handle == 0) {
      _isCompiled = false;
      return false;
    }
    const GLchar* sourcePtr = _source.data();
    glShaderSource(_handle, 1, &sourcePtr, NULL);
    glCompileShader(_handle);
    if (ShaderCompilationStatus() == GL_FALSE) {
      _isCompiled = false;
      LogCompilationMessage();
      return false;
    }
    _isCompiled = true;
    return true;
  }
  auto SubShader::Release() -> void {
    if (_handle != 0)
      glDeleteShader(_handle);
  }
  auto SubShader::ShaderCompilationStatus() -> GLint {
    GLint status;
    glGetShaderiv(_handle, GL_COMPILE_STATUS, &status);
    return status;
  }
  auto SubShader::LogCompilationMessage() -> void {
    GLint stringLength;
    glGetShaderiv(_handle, GL_INFO_LOG_LENGTH, &stringLength);
    std::string message(stringLength, '\0');
    glGetShaderInfoLog(_handle, message.size(), NULL, &message[0]);
    CORE_ERROR("Shader compilation error\nType: {0}\n{1}", _type._to_string(), message);
  }
}  // namespace Engine::GL
