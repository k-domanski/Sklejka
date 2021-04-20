#include "MaterialPanel.h"
#include <ImGui/imgui.h>

using namespace Engine;
namespace fs = std::filesystem;

namespace EditorGUI {
  MaterialPanel::MaterialPanel(): _editorLayer(nullptr) {
    _strBuffer.resize(100);
    _materialsFolder = fs::current_path().string() + "/materials";
  }
  auto MaterialPanel::OnImGuiRender() -> void {
    ImGui::Begin("Material Editor");
    if (ImGui::Button("New Material")) {
      //
      if (!fs::exists(_materialsFolder)) {
        LOG_ERROR("Folder {} does not exist", _materialsFolder.string());
      } else {
        //AssetManager::CreateMaterial();
      }
    }
    ImGui::SameLine();
    ImGui::PushItemWidth(-90);
    ImGui::InputText("Material Name", &_strBuffer[0], _strBuffer.size(),
                     ImGuiInputTextFlags_CharsNoBlank);
    ImGui::Separator();

    DrawMaterialView();

    ImGui::End();
  }
  auto MaterialPanel::SetEditorLayer(EditorLayer* layer) -> void {
    _editorLayer = layer;
  }
  auto MaterialPanel::SetMaterial(const std::shared_ptr< Engine::Renderer::Material >& material)
      -> void {
    _selectedMaterial = material;
  }
  auto MaterialPanel::DrawMaterialView() -> void {
    if (_selectedMaterial == nullptr) {
      ImGui::Text("No material selected");
      return;
    }
  }
}  // namespace EditorGUI