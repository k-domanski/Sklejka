#include "MaterialPanel.h"
#include <ImGui/imgui.h>
#include <Utility/Serializer.h>

using namespace Engine;
namespace fs = std::filesystem;

namespace Editor {
  MaterialPanel::MaterialPanel(): _editorLayer(nullptr) {
    _rootFolder      = fs::current_path();
    auto asset_paths = AssetManager::GetAssetsFolders();
    _materialsFolder = fs::current_path().string() + asset_paths.materials;
    _shadersFolder   = fs::current_path().string() + asset_paths.shaders;
    _texturesFolder  = fs::current_path().string() + asset_paths.textures;
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
    _material = material;
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

      auto material = AssetManager::CreateMaterial(file.string());
      _material     = material;
      SetDirty();
      // TODO: Remove
      SaveMaterial(material);

      std::memset(_charBuffer, 0, 100);
    }
  }
  auto MaterialPanel::DrawMaterialView() -> void {
    if (_material == nullptr) {
      ImGui::Text("No material selected");
      return;
    }
    const ImVec2 button_size{50, 30};
    if (ImGui::Button("Save", button_size)) {
      SaveMaterial(_material);
    }
    ImGui::SameLine();
    if (ImGui::BeginChild("File", {0, button_size.y})) {
      fs::path filePath = _material->FilePath();
      if (fs::exists(filePath)) {
        ImGui::Text("File: %s", filePath.string().c_str());
      } else {
        ImGui::Text("File: < None >");
      }
      ImGui::Text("Asset ID: %s", std::to_string(_material->GetAssetID()).c_str());
    }
    ImGui::EndChild();

    ImGui::Separator();

    ImGui::BeginGroup();
    ShaderSelection();
    PBRParams();
    TextureSelection();
    ColorSelection();
    StencilSetup();
    ImGui::EndGroup();
  }
  auto MaterialPanel::MaterialSelection() -> void {
    const auto has_material = _material != nullptr;
    const std::string preview =
        (has_material ? fs::path(_material->FilePath()).filename().string() : "< None >");
    if (ImGui::BeginCombo("Material", preview.c_str())) {
      for (auto& entry : fs::recursive_directory_iterator(_materialsFolder)) {
        const auto path     = entry.path();
        const auto ext      = path.extension().string();
        const auto filename = path.filename().string();
        if (ext != ".mat")
          continue;
        if (ImGui::Selectable(filename.c_str(),
                              (has_material && fs::path(_material->FilePath()) == path))) {
          // LoadMaterial(path.string());
          _material = AssetManager::GetMaterial(path.string());
        }
      }
      ImGui::EndCombo();
    }
  }
  auto MaterialPanel::ShaderSelection() -> void {
    const auto& shader    = _material->GetShader();
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
          _material->SetShader(AssetManager::GetShader(path.string()));
          SetDirty();
        }
      }
      ImGui::EndCombo();
    }
  }
  auto MaterialPanel::TextureSelection() -> void {
    const auto& main_texture  = _material->GetMainTexture();
    const auto& roughness_map = _material->RoughnessMap();
    const auto& metalness_map = _material->MetalnessMap();

    auto DrawCombo = [&](auto texture, std::string label, auto set_function) -> void {
      const auto has_texture = texture != nullptr;
      const std::string preview =
          (has_texture ? fs::path(texture->FilePath()).filename().string() : "< None >");
      if (ImGui::BeginCombo(label.c_str(), preview.c_str())) {
        /* Select none */
        if (ImGui::Selectable("< None >", !has_texture)) {
          set_function(nullptr);
          //_selectedMaterial->SetMainTexture(AssetManager::GetTexture2D(path.string()));
          SetDirty();
        }
        for (auto& entry : fs::recursive_directory_iterator(_texturesFolder)) {
          const auto path     = entry.path();
          const auto ext      = path.extension().string();
          const auto filename = path.filename().string();
          const auto exts     = {".png", ".jpg"};
          if (!std::any_of(exts.begin(), exts.end(), [ext](const auto& e) { return ext == e; }))
            continue;
          if (ImGui::Selectable(filename.c_str(),
                                (has_texture && fs::path(texture->FilePath()) == path))) {
            set_function(AssetManager::GetTexture2D(path.string()));
            //_selectedMaterial->SetMainTexture(AssetManager::GetTexture2D(path.string()));
            SetDirty();
          }
        }
        ImGui::EndCombo();
      }
    };

    auto tiling = _material->Tiling();
    if (ImGui::SliderFloat2("Tiling", &tiling[0], 0.0f, 4.0f)) {
      _material->Tiling(tiling);
    }
    auto offset = _material->Offset();
    if (ImGui::SliderFloat2("Offset", &offset[0], -1.0f, 1.0f)) {
      _material->Offset(offset);
    }

    DrawCombo(main_texture, "Main Texture",
              [&](auto texture) { _material->SetMainTexture(texture); });
    DrawCombo(roughness_map, "Roughness Map",
              [&](auto texture) { _material->RoughnessMap(texture); });
    DrawCombo(metalness_map, "Metalness Map",
              [&](auto texture) { _material->MetalnessMap(texture); });
  }
  auto MaterialPanel::PBRParams() -> void {
    auto roughness = _material->Roughness();
    if (ImGui::SliderFloat("Roughness", &roughness, 0.0f, 1.0f)) {
      _material->Roughness(roughness);
    }
    auto metalness = _material->Metalness();
    if (ImGui::SliderFloat("Metalness", &metalness, 0.0f, 1.0f)) {
      _material->Metalness(metalness);
    }
  }
  auto MaterialPanel::ColorSelection() -> void {
    auto main_color = _material->MainColor();
    if (ImGui::CollapsingHeader("Main Color##h")) {
      ImGui::ColorPicker4("Main Color##p", &main_color[0]);
      _material->MainColor(main_color);
    }
  }
  template< typename V, typename F >
  struct DrawCombo {
    auto operator()(V& value, const char* label, F set_function) -> void {
      if (ImGui::BeginCombo(label, value._to_string())) {
        for (auto entry : V::_values()) {
          auto selected = value == entry;
          if (ImGui::Selectable(entry._to_string(), selected)) {
            set_function(entry);
          }
        }
        ImGui::EndCombo();
      }
    }
  };
  auto MaterialPanel::StencilSetup() -> void {
    if (ImGui::CollapsingHeader("Stencil")) {
      auto use_stencil = _material->UseStencil();
      if (ImGui::Checkbox("Use Stencil", &use_stencil)) {
        _material->UseStencil(use_stencil);
      }
      int ref = _material->StencilRef();
      if (ImGui::DragInt("Stencil Ref", &ref, 0, 0xFF)) {
        _material->StencilRef(ref);
      }

      auto cmp_func = _material->StencilFunc();
      auto sf_op    = _material->StencilSFail();
      auto zf_op    = _material->StencilZFail();
      auto zp_op    = _material->StencilZPass();

      auto cmp_set = [&](auto func) { _material->StencilFunc(func); };
      auto sf_set  = [&](auto op) { _material->StencilSFail(op); };
      auto zf_set  = [&](auto op) { _material->StencilZFail(op); };
      auto zp_set  = [&](auto op) { _material->StencilZPass(op); };

      DrawCombo< GL::StencilFunc, decltype(cmp_set) >{}(cmp_func, "Compare Function", cmp_set);
      DrawCombo< GL::StencilOp, decltype(sf_set) >{}(sf_op, "Stencil Fail", sf_set);
      DrawCombo< GL::StencilOp, decltype(zf_set) >{}(zf_op, "ZTest Fail", zf_set);
      DrawCombo< GL::StencilOp, decltype(zp_set) >{}(zp_op, "ZTest Pass", zp_set);
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

    _material = AssetManager::GetMaterial(file);
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
        SaveMaterial(_material);
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