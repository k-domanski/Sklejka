#include "pch.h"
#include "Shader.h"
#include <App/Log.h>

namespace Engine::GL {
  auto Shader::GetCurrentHandle() noexcept -> GLuint {
    return s_currentHandle;
  }
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

  auto Shader::FilePath() const noexcept -> std::string {
    return _filePath;
  }

  auto Shader::FilePath(const std::string& filePath) -> std::string {
    return _filePath = filePath;
  }

  auto Shader::GetHandle() const noexcept -> GLuint {
    return _handle;
  }

  auto Shader::AttachShader(const std::shared_ptr< SubShader >& shader) noexcept -> void {
    if (_handle == 0)
      _handle = glCreateProgram();

    auto it = std::find_if(_subShaders.begin(), _subShaders.end(), [&shader](const auto& item) {
      return item->GetHandle() == shader->GetHandle();
    });
    if (_subShaders.size() > 0 && it != _subShaders.end())
      return;
    glAttachShader(_handle, shader->GetHandle());
    _subShaders.push_back(shader);
  }

  auto Shader::Link() noexcept -> bool {
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

  auto Shader::IsValid() noexcept -> bool {
    return _handle != 0u;
  }

  auto Shader::InUse() noexcept -> bool {
    return _handle == s_currentHandle;
  }

  auto Shader::IsLinked() noexcept -> bool {
    return _isLinked;
  }

  auto Shader::Use() noexcept -> void {
    if (InUse() || !IsValid())
      return;
    glUseProgram(_handle);
  }

  auto Shader::BindUniformBlock(const std::string_view& name, GLuint slot) noexcept -> void {
    Use();
    if(auto index = GetUniformBlockIndex(name); index != GL_INVALID_INDEX) {
      glUniformBlockBinding(_handle, index, slot);
    }
  }

  auto Shader::SetValue(const std::string_view& name, int v0) noexcept -> void {
    Use();
    if (auto location = GetUniformLocation(name); location > -1) {
      glUniform1i(location, v0);
    }
  }

  auto Shader::SetValue(const std::string_view& name, int v0, int v1) noexcept -> void {
    Use();
    if (auto location = GetUniformLocation(name); location > -1) {
      glUniform2i(location, v0, v1);
    }
  }

  auto Shader::SetValue(const std::string_view& name, int v0, int v1, int v2) noexcept -> void {
    Use();
    if (auto location = GetUniformLocation(name); location > -1) {
      glUniform3i(location, v0, v1, v2);
    }
  }

  auto Shader::SetValue(const std::string_view& name, int v0, int v1, int v2, int v3) noexcept
      -> void {
    Use();
    if (auto location = GetUniformLocation(name); location > -1) {
      glUniform4i(location, v0, v1, v2, v3);
    }
  }

  auto Shader::SetValue(const std::string_view& name, float v0) noexcept -> void {
    Use();
    if (auto location = GetUniformLocation(name); location > -1) {
      glUniform1f(location, v0);
    }
  }

  auto Shader::SetValue(const std::string_view& name, float v0, float v1) noexcept -> void {
    if (auto location = GetUniformLocation(name); location > -1) {
      glUniform2f(location, v0, v1);
    }
  }

  auto Shader::SetValue(const std::string_view& name, float v0, float v1, float v2) noexcept
      -> void {
    Use();
    if (auto location = GetUniformLocation(name); location > -1) {
      glUniform3f(location, v0, v1, v2);
    }
  }

  auto Shader::SetValue(const std::string_view& name, float v0, float v1, float v2,
                        float v3) noexcept -> void {
    Use();
    if (auto location = GetUniformLocation(name); location > -1) {
      glUniform4f(location, v0, v1, v2, v3);
    }
  }

  auto Shader::SetVector(const std::string_view& name, const glm::vec1& v) noexcept -> void {
    Use();
    if (auto location = GetUniformLocation(name); location > -1) {
      glUniform1fv(location, 1, &v.x);
    }
  }

  auto Shader::SetVector(const std::string_view& name, const glm::vec2& v) noexcept -> void {
    Use();
    if (auto location = GetUniformLocation(name); location > -1) {
      glUniform2fv(location, 1, glm::value_ptr(v));
    }
  }

  auto Shader::SetVector(const std::string_view& name, const glm::vec3& v) noexcept -> void {
    Use();
    if (auto location = GetUniformLocation(name); location > -1) {
      glUniform3fv(location, 1, glm::value_ptr(v));
    }
  }

  auto Shader::SetVector(const std::string_view& name, const glm::vec4& v) noexcept -> void {
    Use();
    if (auto location = GetUniformLocation(name); location > -1) {
      glUniform4fv(location, 1, glm::value_ptr(v));
    }
  }

  auto Shader::SetMatrix(const std::string_view& name, const glm::mat2& m) noexcept -> void {
    Use();
    if (auto location = GetUniformLocation(name); location > -1) {
      glUniformMatrix2fv(location, 1, GL_FALSE, glm::value_ptr(m));
    }
  }

  auto Shader::SetMatrix(const std::string_view& name, const glm::mat3& m) noexcept -> void {
    Use();
    if (auto location = GetUniformLocation(name); location > -1) {
      glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(m));
    }
  }

  auto Shader::SetMatrix(const std::string_view& name, const glm::mat4& m) noexcept -> void {
    Use();
    if (auto location = GetUniformLocation(name); location > -1) {
      glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(m));
    }
  }

  auto Shader::Release() noexcept -> void {
    if (_handle != 0)
      glDeleteProgram(_handle);
  }

  auto Shader::ShaderLinkStatus() noexcept -> GLint {
    GLint param;
    glGetProgramiv(_handle, GL_LINK_STATUS, &param);
    return param;
  }

  auto Shader::LogLinkMessage() noexcept -> void {
    GLint stringLength;
    glGetProgramiv(_handle, GL_INFO_LOG_LENGTH, &stringLength);
    std::string message(stringLength, '\0');
    glGetProgramInfoLog(_handle, message.size(), NULL, &message[0]);
    CORE_ERROR("Shader link error\n{0}", message);
  }

  auto Shader::GetUniformLocation(const std::string_view& name) noexcept -> GLint {
    if (_uniformCache.count(name) != 0)
      return _uniformCache[name];
    return _uniformCache[name] = glGetUniformLocation(_handle, name.data());
  }

  auto Shader::GetUniformBlockIndex(const std::string_view& name) noexcept -> GLuint {
    if (_uniformBlockCache.count(name) != 0) {
      return _uniformBlockCache[name];
    }
    auto index = glGetUniformBlockIndex(_handle, name.data());
    if (index == GL_INVALID_INDEX)
      return index;
    return _uniformBlockCache[name] = index;
  }

}  // namespace Engine::GL