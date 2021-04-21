#include "pch.h"
#include "MeshRenderer.h"
namespace Engine::Components {
  auto MeshRenderer::GetModel() -> std::shared_ptr< Renderer::Model > {
    return _model;
  }

  auto MeshRenderer::SetModel(std::shared_ptr< Renderer::Model > model) -> void {
    _model  = model;
    _dirty = true;
  }

  auto MeshRenderer::GetMaterial() -> std::shared_ptr< Renderer::Material > {
    return _material;
  }

  auto MeshRenderer::SetMaterial(std::shared_ptr< Renderer::Material > material) -> void {
    _material = material;
    _dirty    = true;
  }

  auto MeshRenderer::IsDirty() -> bool {
    return _dirty;
  }

  void MeshRenderer::SetDirty(bool dirty) {
    _dirty = dirty;
  }

  std::string MeshRenderer::SaveToJson(std::string filePath)
  {
    using namespace nlohmann;
    json json = nlohmann::json{{"componentType", "meshRenderer"},
      {"model", _model->GetFilepath()},
        {"material", std::to_string(_material->GetAssetID())}};

    std::ofstream ofstream;
    ofstream.open(filePath);
    ofstream << json.dump(4);
    ofstream.close();

    return json.dump(4);
  }
}  // namespace Engine::Components
