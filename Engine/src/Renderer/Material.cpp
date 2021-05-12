#include "pch.h"
#include "Material.h"

#include "App/AssetManager.h"
#include "nlohmann/json.hpp"

namespace Engine::Renderer {
  Material::Material(std::size_t assetID)
      : _assetID(assetID), _shader(nullptr), _mainTexture(nullptr) {
  }

  auto Material::SetShader(const std::shared_ptr< GL::Shader >& shader) noexcept -> void {
    _shader = shader;
  }
  auto Material::SetMainTexture(const std::shared_ptr< GL::Texture2D >& mainTexture) noexcept
      -> void {
    _mainTexture = mainTexture;
  }
  std::shared_ptr< GL::Shader > Material::GetShader() {
    return _shader;
  }
  std::shared_ptr< GL::Texture2D > Material::GetMainTexture() {
    return _mainTexture;
  }
  auto Material::MainColor() const noexcept -> glm::vec4 {
    return _mainColor;
  }
  auto Material::MainColor(const glm::vec4& color) noexcept -> glm::vec4 {
    return _mainColor = color;
  }
  auto Material::MainColorPtr() -> float* {
    return &_mainColor[0];
  }
  auto Material::Queue() const noexcept -> uint32_t {
    return _queue;
  }
  auto Material::Queue(uint32_t queue) noexcept -> uint32_t {
    return _queue = queue;
  }
  std::size_t Material::GetAssetID() {
    return _assetID;
  }
  std::string Material::ToJson() {
    using namespace nlohmann;
    nlohmann::json json = nlohmann::json{
        {"assetID", std::to_string(_assetID)},
        {"shaderFilepath", (_shader != nullptr ? _shader->FilePath() : "")},
        {"diffuseFilepath", (_mainTexture != nullptr ? _mainTexture->FilePath() : "")},
        {"mainColor", json::array({_mainColor.r, _mainColor.g, _mainColor.b, _mainColor.a})}};

    return json.dump(4);
  }
  auto Material::FilePath() const noexcept -> std::string {
    return _filePath;
  }
  auto Material::FilePath(const std::string& filePath) noexcept -> std::string {
    return _filePath = filePath;
  }
  auto Material::Use() noexcept -> void {
    if (_mainTexture != nullptr) {
      _mainTexture->Bind(0);
    }
    if (_shader != nullptr) {
      SetShaderData();
    }
  }
  auto Material::SetShaderData() noexcept -> void {
    _shader->Use();
    _shader->SetValue("u_MainTexture", 0);
    // TODO: Use uniform buffer if we have a lot of data
    _shader->SetVector("u_Color", _mainColor);
  }
}  // namespace Engine::Renderer
