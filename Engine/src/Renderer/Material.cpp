#include "pch.h"
#include "Material.h"


#include "App/AssetManager.h"
#include "nlohmann/json.hpp"

namespace Engine::Renderer {
  Material::Material(std::size_t assetID)
      : _assetID(assetID), _shader(nullptr), _mainTexture(nullptr) {
  }

  auto Material::SetShader(const std::shared_ptr< GL::Shader >& shader,
                           const std::string& filePath) noexcept -> void {
    _shader     = shader;
    _shaderFile = filePath;
  }
  auto Material::SetMainTexture(const std::shared_ptr< GL::Texture2D >& mainTexture,
                                const std::string& filePath) noexcept -> void {
    _mainTexture = mainTexture;
    _diffuseFile = filePath;
  }

  auto Material::SetTransform(glm::mat4 m) -> void {
    _shader->Use();
    _shader->SetMatrix("u_model_matrix", m);
  }

  std::shared_ptr< GL::Shader > Material::GetShader() {
    return _shader;
  }

  std::shared_ptr< GL::Texture2D > Material::GetDiffuse() {
    return _mainTexture;
  }

  std::string Material::GetShaderFilepath()
  {
    return _shaderFile;
  }

  std::string Material::GetDiffuseFilepath()
  {
    return _diffuseFile;
  }

  std::size_t Material::GetAssetID() {
    return _assetID;
  }

  std::string Material::ToJson()
  {
    nlohmann::json json = nlohmann::json{{"assetID", std::to_string(_assetID)},
                                         {"shaderFilepath", _shaderFile},
                                         {"diffuseFilepath", _diffuseFile}};

    return json.dump(4);
  }

  auto Material::Use() noexcept -> void {
    if (_mainTexture != nullptr) {
      _mainTexture->Bind(0);
    }
    if (_shader != nullptr) {
      _shader->Use();
      _shader->SetValue("u_mainTexture", 0);
    }
  }
}  // namespace Engine::Renderer
