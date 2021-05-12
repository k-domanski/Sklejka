#include "MaterialPanel.h"
#include <ImGui/imgui.h>
#include <Utility/Serializer.h>

using namespace Engine;
namespace fs = std::filesystem;

namespace Editor {
  MaterialPanel::MaterialPanel(): _editorLayer(nullptr) {
    _rootFolder      = fs::current_path();
    _materialsFolder = fs::current_path().string() + "\\materials\\";
    _shadersFolder   = fs::current_path().string() + "\\shaders\\";
    _texturesFolder  = fs::current_path().string() + "\\textures\\";
  }
  auto MaterialPanel::OnImGuiRender() -> void {
    ImGui::Begin("Material Editor");

    CreateMaterialButton();
    MaterialSelection();

    ImGui::Separator();
    ImGui::BeginChild("Material Attributes");
    DrawMaterialView();
    ImGui::EndChild();
    /* Drag and Drop */
    if (ImGui::BeginDragDropTarget()) {
      if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("FILE")) {
        auto payload_str = std::string(static_cast< char* >(payload->Data));
        LOG_TRACE("Drag n Drop: {}", payload_str);
        LoadMaterial(payload_str);
      }
      ImGui::EndDragDropTarget();
    }
    ImGui::End();
  }
  auto MaterialPanel::SetEditorLayer(EditorLayer* layer) -> void {
    _editorLayer = layer;
  }
  auto MaterialPanel::SetMaterial(const std::shared_ptr< Engine::Renderer::Material >& material)
      -> void {
    _selectedMaterial = material;
  }
  auto MaterialPanel::CreateMaterialButton() -> void {
    auto pressed = ImGui::Button("New Material");
    ImGui::SameLine();
    ImGui::PushItemWidth(-90);
    const ImGuiInputTextFlags flags = ImGuiInputTextFlags_CharsNoBlank;
    ImGui::InputText("Material Name", _charBuffer, 100, flags);

    if (pressed) {
      if (!fs::exists(_materialsFolder)) {
        LOG_ERROR("Folder {} does not exist", _materialsFolder.string());
        return;
      }
      auto strBuff = std::string(_charBuffer);
      if (strBuff.size() == 0) {
        LOG_ERROR("No file name provided");
        return;
      }

      fs::path file = _materialsFolder.string() + _charBuffer + ".mat";
      if (fs::exists(file)) {
        LOG_TRACE("File {} already exists", file.filename());
        return;
      }
      LOG_TRACE("File: {}", file);

      auto material     = AssetManager::CreateMaterial(file.string());
      _selectedMaterial = material;
      SetDirty();
      // TODO: Remove
      SaveMaterial(material);

      std::memset(_charBuffer, 0, 100);
    }
  }
  auto MaterialPanel::DrawMaterialView() -> void {
    if (_selectedMaterial == nullptr) {
      ImGui::Text("No material selected");
      return;
    }
    const ImVec2 button_size{50, 30};
    if (ImGui::Button("Save", button_size)) {
      SaveMaterial(_selectedMaterial);
    }
    ImGui::SameLine();
    if (ImGui::BeginChild("File", {0, button_size.y})) {
      fs::path filePath = _selectedMaterial->FilePath();
      if (fs::exists(filePath)) {
        ImGui::Text("File: %s", filePath.string().c_str());
      } else {
        ImGui::Text("File: < None >");
      }
      ImGui::Text("Asset ID: %s", std::to_string(_selectedMaterial->GetAssetID()).c_str());
    }
    ImGui::EndChild();

    ImGui::Separator();

    ImGui::BeginGroup();
    ShaderSelection();
    PBRParams();
    TextureSelection();
    ColorSelection();
    ImGui::EndGroup();
  }
  auto MaterialPanel::MaterialSelection() -> void {
    const auto has_material = _selectedMaterial != nullptr;
    const std::string preview =
        (has_material ? fs::path(_selectedMaterial->FilePath()).filename().string() : "< None >");
    if (ImGui::BeginCombo("Material", preview.c_str())) {
      for (auto& entry : fs::recursive_directory_iterator(_materialsFolder)) {
        const auto path     = entry.path();
        const auto ext      = path.extension().string();
        const auto filename = path.filename().string();
        if (ext != ".mat")
          continue;
        if (ImGui::Selectable(filename.c_str(),
                              (has_material && fs::path(_selectedMaterial->FilePath()) == path))) {
          LoadMaterial(path.string());
        }
      }
      ImGui::EndCombo();
    }
  }
  auto MaterialPanel::ShaderSelection() -> void {
    const auto& shader    = _selectedMaterial->GetShader();
    const auto has_shader = shader != nullptr;
    const std::string preview =
        (has_shader ? fs::path(shader->FilePath()).filename().string() : "< None >");
    if (ImGui::BeginCombo("Shader", preview.c_str())) {
      for (auto& entry : fs::recursive_directory_iterator(_shadersFolder)) {
        const auto path     = entry.path();
        const auto ext      = path.extension().string();
        const auto filename = path.filename().string();
        if (ext != ".glsl")
          continue;
        if (ImGui::Selectable(filename.c_str(),
                              (has_shader && fs::path(shader->FilePath()) == path))) {
          _selectedMaterial->SetShader(AssetManager::GetShader(path.string()));
          SetDirty();
        }
      }
      ImGui::EndCombo();
    }
  }
  auto MaterialPanel::TextureSelection() -> void {
    const auto& texture    = _selectedMaterial->GetMainTexture();
    const auto has_texture = texture != nullptr;
    const std::string preview =
        (has_texture ? fs::path(texture->FilePath()).filename().string() : "< None >");
    if (ImGui::BeginCombo("Texture", preview.c_str())) {
      for (auto& entry : fs::recursive_directory_iterator(_texturesFolder)) {
        const auto path     = entry.path();
        const auto ext      = path.extension().string();
        const auto filename = path.filename().string();
        const auto exts     = {".png", ".jpg"};
        if (!std::any_of(exts.begin(), exts.end(), [ext](const auto& e) { return ext == e; }))
          continue;
        if (ImGui::Selectable(filename.c_str(),
                              (has_texture && fs::path(texture->FilePath()) == path))) {
          _selectedMaterial->SetMainTexture(AssetManager::GetTexture2D(path.string()));
          SetDirty();
        }
      }
      ImGui::EndCombo();
    }
  }
  auto MaterialPanel::PBRParams() -> void {
    auto roughness = _selectedMaterial->Roughness();
    if (ImGui::SliderFloat("Roughness", &roughness, 0.0f, 1.0f)) {
      _selectedMaterial->Roughness(roughness);
    }
    auto metalness = _selectedMaterial->Metalness();
    if (ImGui::SliderFloat("Metalness", &metalness, 0.0f, 1.0f)) {
      _selectedMaterial->Metalness(metalness);
    }
  }
  auto MaterialPanel::ColorSelection() -> void {
    if (ImGui::CollapsingHeader("Main Color##h")) {
      ImGui::ColorPicker4("Main Color##p", _selectedMaterial->MainColorPtr());
    }
  }
  auto MaterialPanel::SaveMaterial(const std::shared_ptr< Renderer::Material >& material) -> void {
    if (Serializer::SaveMaterial(material)) {
      ClearDirty();
    }
  }
  auto MaterialPanel::LoadMaterial(const std::string& file) -> void {
    auto filePath = fs::path(file);
    if (filePath.extension() != ".mat") {
      // Silent ignore
      return;
    }
    if (!fs::exists(filePath)) {
      LOG_ERROR("File does not exist: {}", file);
      return;
    }

    _selectedMaterial = AssetManager::GetMaterial(file);
    ClearDirty();
  }
  auto MaterialPanel::ShowSaveModal(const std::string& loadFile) -> void {
    // if (!ImGui::IsPopupOpen("Save material?"))
    /*ImGui::OpenPopup("Save material?");*/

    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

    if (ImGui::BeginPopupModal("Save material?")) {
      ImGui::Text("Current material has been modified.\nDo you want do save your changes?");
      ImGui::SetItemDefaultFocus();
      if (ImGui::Button("Save")) {
        SaveMaterial(_selectedMaterial);
        ImGui::CloseCurrentPopup();
      }
      if (ImGui::Button("Discard")) {
        ClearDirty();
        LoadMaterial(loadFile);
        ImGui::CloseCurrentPopup();
      }
      ImGui::EndPopup();
    }
  }
  auto MaterialPanel::IsDirty() -> bool {
    return _isDirty;
  }
  auto MaterialPanel::SetDirty() -> void {
    _isDirty = true;
  }
  auto MaterialPanel::ClearDirty() -> void {
    _isDirty = false;
  }
}  // namespace Editor