#pragma once
#include <pch.h>
#include <GL/GLCore.h>
#include <nlohmann/json.hpp>

namespace Engine::Renderer {
  class Material {
  private:
    std::size_t _assetID;
    std::string _filePath;
    uint32_t _queue{1000};
    std::shared_ptr< GL::Shader > _shader;
    std::shared_ptr< GL::Texture2D > _mainTexture;
    glm::vec4 _mainColor{1.0f, 1.0f, 1.0f, 1.0f};

  public:
    Material(std::size_t assetID);
    auto SetShader(const std::shared_ptr< GL::Shader >& shader) noexcept -> void;
    auto SetMainTexture(const std::shared_ptr< GL::Texture2D >& mainTexture) noexcept -> void;

    auto GetShader() -> std::shared_ptr< GL::Shader >;
    auto GetMainTexture() -> std::shared_ptr< GL::Texture2D >;
    auto MainColor() const noexcept -> glm::vec4;
    auto MainColor(const glm::vec4& color) noexcept -> glm::vec4;
    auto MainColorPtr() -> float*;
    auto Queue() const noexcept -> uint32_t;
    auto Queue(uint32_t queue) noexcept -> uint32_t;

    std::size_t GetAssetID();
    std::string ToJson();
    auto FilePath() const noexcept -> std::string;
    auto FilePath(const std::string& filePath) noexcept -> std::string;
    auto Use() noexcept -> void;
    auto SetShaderData() noexcept -> void;
  };
}  // namespace Engine::Renderer