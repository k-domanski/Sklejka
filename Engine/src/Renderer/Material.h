#pragma once
#include <pch.h>
#include <GL/GLCore.h>
#include <nlohmann/json.hpp>

namespace Engine::Renderer {
  class Material {
  private:
    std::size_t _assetID;
    std::string _filePath;
    std::shared_ptr< GL::Shader > _shader;
    std::shared_ptr< GL::Texture2D > _mainTexture;
    // TODO: std::vector<EntityID> _assignedEntities

  public:
    Material(std::size_t assetID);
    auto SetShader(const std::shared_ptr< GL::Shader >& shader) noexcept -> void;
    auto SetMainTexture(const std::shared_ptr< GL::Texture2D >& mainTexture) noexcept -> void;
    auto SetTransform(glm::mat4 m) -> void;
    std::shared_ptr< GL::Shader > GetShader();
    std::shared_ptr< GL::Texture2D > GetMainTexture();
    std::size_t GetAssetID();
    std::string ToJson();
    auto FilePath() const noexcept -> std::string;
    auto FilePath(const std::string& filePath) noexcept -> std::string;
    auto Use() noexcept -> void;
  };
}  // namespace Engine::Renderer