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
  auto Material::RoughnessMap() const noexcept -> std::shared_ptr< GL::Texture2D > {
    return _roughnessMap;
  }
  auto Material::RoughnessMap(const std::shared_ptr< GL::Texture2D >& map) noexcept
      -> std::shared_ptr< GL::Texture2D > {
    return _roughnessMap = map;
  }
  auto Material::MetalnessMap() const noexcept -> std::shared_ptr< GL::Texture2D > {
    return _metalnessMap;
  }
  auto Material::MetalnessMap(const std::shared_ptr< GL::Texture2D >& map) noexcept
      -> std::shared_ptr< GL::Texture2D > {
    return _metalnessMap = map;
  }
  auto Material::MainColor() const noexcept -> glm::vec4 {
    return _mainColor;
  }
  auto Material::MainColor(const glm::vec4& color) noexcept -> glm::vec4 {
    return _mainColor = color;
  }
  auto Material::Roughness() const noexcept -> float {
    return _roughness;
  }
  auto Material::Roughness(float value) noexcept -> float {
    return _roughness = value;
  }
  auto Material::Metalness() const noexcept -> float {
    return _metalness;
  }
  auto Material::Metalness(float value) noexcept -> float {
    return _metalness = value;
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
  auto Material::UseStencil() const noexcept -> bool {
    return _useStencil;
  }
  auto Material::UseStencil(bool value) noexcept -> bool {
    return _useStencil = value;
  }
  auto Material::StencilRef() const noexcept -> uint8_t {
    return _stencilRef;
  }
  auto Material::StencilRef(uint8_t ref) noexcept -> uint8_t {
    return _stencilRef = ref;
  }
  auto Material::StencilFunc() const noexcept -> GL::StencilFunc {
    return _stencilFunc;
  }
  auto Material::StencilFunc(GL::StencilFunc func) noexcept -> GL::StencilFunc {
    return _stencilFunc = func;
  }
  auto Material::StencilSFail() const noexcept -> GL::StencilOp {
    return _sfail;
  }
  auto Material::StencilSFail(GL::StencilOp op) noexcept -> GL::StencilOp {
    return _sfail = op;
  }
  auto Material::StencilZFail() const noexcept -> GL::StencilOp {
    return _zfail;
  }
  auto Material::StencilZFail(GL::StencilOp op) noexcept -> GL::StencilOp {
    return _zfail = op;
  }
  auto Material::StencilZPass() const noexcept -> GL::StencilOp {
    return _zpass;
  }
  auto Material::StencilZPass(GL::StencilOp op) noexcept -> GL::StencilOp {
    return _zpass = op;
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
        {"roughnessMap", (_roughnessMap != nullptr ? _roughnessMap->FilePath() : "")},
        {"metalnessMap", (_metalnessMap != nullptr ? _metalnessMap->FilePath() : "")},
        {"mainColor", json::array({_mainColor.r, _mainColor.g, _mainColor.b, _mainColor.a})},
        {"roughness", _roughness},
        {"metalness", _metalness},
        {"useStencil", _useStencil},
        {"stencilRef", _stencilRef},
        {"stencilFunc", _stencilFunc._to_string()},
        {"sfail", _sfail._to_string()},
        {"zfail", _zfail._to_string()},
        {"zpass", _zpass._to_string()}
        /**/};

    return json.dump(4);
  }
  auto Material::FilePath() const noexcept -> std::string {
    return _filePath;
  }
  auto Material::FilePath(const std::string& filePath) noexcept -> std::string {
    return _filePath = filePath;
  }
  auto Material::Use() noexcept -> void {
    if (_assetID == s_currentMaterial) {
      return;
    }

    GL::Context::StencilTest(_useStencil);
    if (_useStencil) {
      GL::Context::StencilFunction(_stencilFunc, _stencilRef);
      GL::Context::StencilOperation(_sfail, _zfail, _zpass);
    }

    auto white_texture = AssetManager::GetTexture2D("./textures/white.png");
    if (_mainTexture != nullptr) {
      _mainTexture->Bind(0);
    } else {
      white_texture->Bind(0);
    }
    if (_roughnessMap != nullptr) {
      _roughnessMap->Bind(1);
    } else {
      white_texture->Bind(1);
    }
    if (_metalnessMap != nullptr) {
      _metalnessMap->Bind(2);
    } else {
      white_texture->Bind(2);
    }
    if (_shader != nullptr) {
      SetShaderData();
    }
    s_currentMaterial = _assetID;
  }
  auto Material::SetShaderData() noexcept -> void {
    _shader->Use();
    _shader->SetValue("u_MainTexture", 0);
    _shader->SetValue("u_RoughnessMap", 1);
    _shader->SetValue("u_MetalnessMap", 2);

    // TODO: Use uniform buffer if we have a lot of data
    _shader->SetVector("u_Color", _mainColor);
    _shader->SetValue("u_Roughness", _roughness);
    _shader->SetValue("u_Metalness", _metalness);
  }
}  // namespace Engine::Renderer
