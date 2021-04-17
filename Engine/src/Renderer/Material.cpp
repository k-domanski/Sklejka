#include "pch.h"
#include "Material.h"

namespace Engine::Renderer {
  Material::Material(std::size_t assetID): _assetID(assetID), _shader(nullptr), _diffuse(nullptr) {
  }
  auto Material::SetShader(const std::shared_ptr< GL::Shader >& shader,
                           const std::string& filePath) noexcept -> void {
    _shader     = shader;
    _shaderFile = filePath;
  }
  auto Material::SetDiffuse(const std::shared_ptr< GL::Texture2D >& diffuse,
                            const std::string& filePath) noexcept -> void {
    _diffuse     = diffuse;
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
    return _diffuse;
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

  auto Material::Use() noexcept -> void {
    if (_diffuse != nullptr) {
      _diffuse->Bind(0);
    }
    if (_shader != nullptr) {
      _shader->Use();
      _shader->SetValue("u_mainTexture", 0);
    }
  }
}  // namespace Engine::Renderer
