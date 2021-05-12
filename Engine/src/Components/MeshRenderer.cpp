#include "pch.h"
#include "MeshRenderer.h"

#include "../../../Editor/src/ImGui/Panels/FileSystemPanel.h"

namespace Engine::Components {
  auto MeshRenderer::GetModel() -> std::shared_ptr< Renderer::Model > {
    return _model;
  }

  auto MeshRenderer::GetMeshIndex() -> int {
    return _meshIndex;
  }

  auto MeshRenderer::SetModel(std::shared_ptr< Renderer::Model > model) -> void {
    _model = model;
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

  std::string MeshRenderer::SaveToJson(std::string filePath) {
    std::ofstream ofstream;
    ofstream.open(filePath);
    ofstream << SaveToJson();
    ofstream.close();

    return SaveToJson();
  }

  std::string MeshRenderer::SaveToJson() {
    using namespace nlohmann;
    json json = nlohmann::json{
        {"componentType", "meshRenderer"},
        {"model", Utility::StripToRelativePath(_model->GetFilepath())},
        {"meshIndex", _meshIndex},
        {"material",
         _material != nullptr ? Utility::StripToRelativePath(_material->FilePath()) : ""}};

    return json.dump(4);
  }

  auto MeshRenderer::LoadFromJson(std::string filePath) -> void {
    nlohmann::json json;
    if (filePath[0] == '{' || filePath[0] == '\n')  // HACK: Check if string is json
      json = nlohmann::json::parse(filePath.begin(), filePath.end());
    else {
      auto content = Utility::ReadTextFile(filePath);
      json         = nlohmann::json::parse(content.begin(), content.end());
    }

    _model    = AssetManager::GetModel(json["model"]);
    _material = AssetManager::GetMaterial(std::string(json["material"]));
    if (json.count("meshIndex") != 0) {
      _meshIndex = json["meshIndex"];
    } else {
      _meshIndex = 0;
    }
  }

}  // namespace Engine::Components
