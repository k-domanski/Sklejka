#pragma once
#include <pch.h>
#include <GL/SubShader.h>

namespace Engine::GL {
  class Shader {
  private:
    static inline GLuint s_currentHandle = 0u;
    GLuint _handle                       = 0;
    bool _isLinked                       = false;
    std::vector< std::shared_ptr< SubShader > > _subShaders;
    std::unordered_map< std::string_view, GLuint > _uniformCache;
    std::unordered_map< std::string_view, GLuint > _uniformBlockCache;

  public:
    static auto GetCurrentHandle() noexcept -> GLuint;
    Shader()              = default;
    Shader(const Shader&) = delete;
    auto operator=(const Shader&) -> Shader& = delete;
    Shader(Shader&& other) noexcept;
    ~Shader();
    auto operator=(Shader&& other) noexcept -> Shader&;
    auto GetHandle() const noexcept -> GLuint;
    auto AttachShader(const std::shared_ptr< SubShader >& shader) noexcept -> void;
    auto Link() noexcept -> bool;
    auto IsValid() noexcept -> bool;
    auto InUse() noexcept -> bool;
    auto IsLinked() noexcept -> bool;
    auto Use() noexcept -> void;

    auto BindUniformBlock(const std::string_view& name, GLuint slot) noexcept -> void;

    auto SetValue(const std::string_view& name, int v0) noexcept -> void;
    auto SetValue(const std::string_view& name, int v0, int v1) noexcept -> void;
    auto SetValue(const std::string_view& name, int v0, int v1, int v2) noexcept -> void;
    auto SetValue(const std::string_view& name, int v0, int v1, int v2, int v3) noexcept -> void;
    auto SetValue(const std::string_view& name, float v0) noexcept -> void;
    auto SetValue(const std::string_view& name, float v0, float v1) noexcept -> void;
    auto SetValue(const std::string_view& name, float v0, float v1, float v2) noexcept -> void;
    auto SetValue(const std::string_view& name, float v0, float v1, float v2, float v3) noexcept
        -> void;
    auto SetVector(const std::string_view& name, const glm::vec1& v) noexcept -> void;
    auto SetVector(const std::string_view& name, const glm::vec2& v) noexcept -> void;
    auto SetVector(const std::string_view& name, const glm::vec3& v) noexcept -> void;
    auto SetVector(const std::string_view& name, const glm::vec4& v) noexcept -> void;
    auto SetMatrix(const std::string_view& name, const glm::mat2& m) noexcept -> void;
    auto SetMatrix(const std::string_view& name, const glm::mat3& m) noexcept -> void;
    auto SetMatrix(const std::string_view& name, const glm::mat4& m) noexcept -> void;

  private:
    auto Release() noexcept -> void;
    auto ShaderLinkStatus() noexcept -> GLint;
    auto LogLinkMessage() noexcept -> void;
    auto GetUniformLocation(const std::string_view& name) noexcept -> GLint;
    auto GetUniformBlockIndex(const std::string_view& name) noexcept -> GLuint;
  };
}  // namespace Engine::GL