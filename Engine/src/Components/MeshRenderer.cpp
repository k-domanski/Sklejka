#include "pch.h"
#include "MeshRenderer.h"

#include "../../../Editor/src/ImGui/Panels/FileSystemPanel.h"

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
    std::ofstream ofstream;
    ofstream.open(filePath);
    ofstream << SaveToJson();
    ofstream.close();

    return SaveToJson();
  }

  std::string MeshRenderer::SaveToJson()
  {
    using namespace nlohmann;
    json json = nlohmann::json{{"componentType", "meshRenderer"},
      {"model", _model->GetFilepath()},
        {"material", std::to_string(_material->GetAssetID())}};

    return json.dump(4);
  }

  auto MeshRenderer::LoadFromJson(std::string filePath) -> void
  {
    auto content        = Utility::ReadTextFile(filePath);
    nlohmann::json json = nlohmann::json::parse(content.begin(), content.end());

    std::string material_assetID_string = json["material"];
    std::stringstream ss(material_assetID_string);
    size_t material_assetID;
    ss >> material_assetID;

    _model = AssetManager::GetModel(json["model"]);
    _material = AssetManager::GetMaterial(material_assetID);
  }
}  // namespace Engine::Components
