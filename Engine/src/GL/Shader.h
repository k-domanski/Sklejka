#pragma once
#include <pch.h>
#include <GL/SubShader.h>

namespace Engine::GL {
  class Shader : public IGLResource {
  private:
    GLuint _handle = 0;
    std::vector< std::shared_ptr< SubShader > > _subShaders;
    bool _isLinked = false;

  public:
    Shader()              = default;
    Shader(const Shader&) = delete;
    auto operator=(const Shader&) -> Shader& = delete;
    Shader(Shader&& other) noexcept;
    ~Shader();
    auto operator=(Shader&& other) noexcept -> Shader&;
    auto GetHandle() const noexcept -> GLuint override;
    auto AttachShader(const std::shared_ptr< SubShader >& shader) -> void;
    auto Link() -> bool;

  private:
    auto Release() -> void;
    auto ShaderLinkStatus() -> GLint;
    auto LogLinkMessage() -> void;
  };
}  // namespace Engine::GL