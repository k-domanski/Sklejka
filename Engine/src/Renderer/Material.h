#pragma once
#include <pch.h>
#include <GL/GLCore.h>
#include <nlohmann/json.hpp>


namespace Engine::Renderer {
  class Material {
  private:
    std::size_t _assetID;
    std::shared_ptr< GL::Shader > _shader;
    std::shared_ptr< GL::Texture2D > _mainTexture;
    std::string _shaderFile;
    std::string _diffuseFile;
    // TODO: std::vector<EntityID> _assignedEntities

  public:
    Material(std::size_t assetID);
    Material(nlohmann::json json);
    auto SetShader(const std::shared_ptr< GL::Shader >& shader,
                   const std::string& filePath) noexcept -> void;
    auto SetMainTexture(const std::shared_ptr< GL::Texture2D >& mainTexture,
                    const std::string& filePath) noexcept -> void;
    auto SetTransform(glm::mat4 m) -> void;
    std::shared_ptr< GL::Shader > GetShader();
    std::shared_ptr< GL::Texture2D > GetDiffuse();
    std::string GetShaderFilepath();
    std::string GetDiffuseFilepath();
    std::size_t GetAssetID();
    std::string ToJson();
    auto Use() noexcept -> void;
  };
}  // namespace Engine::Renderer