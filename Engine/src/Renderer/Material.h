#pragma once
#include <pch.h>
#include <GL/GLCore.h>

namespace Engine::Renderer {
  class Material {
  private:
    std::size_t _assetID;
    std::shared_ptr< GL::Shader > _shader;
    std::shared_ptr< GL::Texture2D > _diffuse;
    std::string _shaderFile;
    std::string _diffuseFile;
    // TODO: std::vector<EntityID> _assignedEntities

  public:
    Material(std::size_t assetID);
    auto SetShader(const std::shared_ptr< GL::Shader >& shader,
                   const std::string& filePath) noexcept -> void;
    auto SetDiffuse(const std::shared_ptr< GL::Texture2D >& diffuse,
                    const std::string& filePath) noexcept -> void;
    auto SetTransform(glm::mat4 m) -> void;
    auto Use() noexcept -> void;
  };
}  // namespace Engine::Renderer