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
    _diffuse = diffuse;
    _diffuseFile = filePath;
  }
  auto Material::Use() noexcept -> void {
    _shader->Use();
    _diffuse->Bind(0);
    _shader->SetValue("u_diffuse", 0);
  }
}  // namespace Engine::Renderer