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

  auto Material::SetTransform(glm::mat4 m) -> void {
    _shader->Use();
    _shader->SetMatrix("u_model_matrix", m);
  }

  std::shared_ptr< GL::Shader > Material::GetShader() {
    return _shader;
  }

  std::shared_ptr< GL::Texture2D > Material::GetMainTexture() {
    return _mainTexture;
  }
  std::size_t Material::GetAssetID() {
    return _assetID;
  }
  std::string Material::ToJson() {
    nlohmann::json json = nlohmann::json{
        {"assetID", std::to_string(_assetID)},
        {"shaderFilepath", (_shader != nullptr ? _shader->FilePath() : "")},
        {"diffuseFilepath", (_mainTexture != nullptr ? _mainTexture->FilePath() : "")}};

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
    _shader->SetValue("u_mainTexture", 0);
    // TODO: Use uniform buffer if we have a lot of data
    _shader->SetVector("u_mainColor", _mainColor);
  }
}  // namespace Engine::Renderer
