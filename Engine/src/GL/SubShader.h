#pragma once
#include <pch.h>
#include <GL/IGLResource.h>

namespace Engine::GL {
  BETTER_ENUM(__ShaderType, GLenum, VertexShader = GL_VERTEX_SHADER,
              FragmentShader = GL_FRAGMENT_SHADER);

  typedef __ShaderType ShaderType;

  class SubShader : public IGLResource {
  private:
    GLuint _handle = 0;
    std::string _source;
    bool _isCompiled = false;
    ShaderType _type;

  public:
    SubShader(ShaderType type, std::string source) noexcept;
    SubShader(SubShader&& other) noexcept;
    auto operator=(SubShader&& other) noexcept -> SubShader&;
    auto operator=(const SubShader& other) -> SubShader& = delete;
    SubShader(const SubShader& other)                    = delete;
    ~SubShader();
    auto GetSource() const -> const std::string&;
    auto GetType() const -> ShaderType;
    auto GetHandle() const noexcept -> GLuint override;
    auto Compile() -> bool;

  private:
    auto Release() -> void;
    auto ShaderCompilationStatus() -> GLint;
    auto LogCompilationMessage() -> void;
  };
}  // namespace Engine::GL
