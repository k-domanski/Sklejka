#pragma once
#include "Engine.h"
#include <filesystem>

class EditorLayer;

namespace EditorGUI {
  class MaterialPanel {
  private:
    EditorLayer* _editorLayer;
    char _charBuffer[100]{0};
    bool _isDirty{false};
    std::shared_ptr< Engine::Renderer::Material > _selectedMaterial;
    std::filesystem::path _rootFolder;
    std::filesystem::path _materialsFolder;
    std::filesystem::path _shadersFolder;
    std::filesystem::path _texturesFolder;

  public:
    MaterialPanel();
    auto OnImGuiRender() -> void;
    auto SetEditorLayer(EditorLayer* layer) -> void;
    auto SetMaterial(const std::shared_ptr< Engine::Renderer::Material >& material) -> void;

  private:
    auto CreateMaterialButton() -> void;
    auto DrawMaterialView() -> void;
    auto MaterialSelection() -> void;
    auto ShaderSelection() -> void;
    auto TextureSelection() -> void;
    auto SaveMaterial(const std::shared_ptr< Engine::Renderer::Material >& material) -> void;
    auto LoadMaterial(const std::string& file) -> void;
    // Not Working
    auto ShowSaveModal(const std::string& loadFile) -> void;
    auto IsDirty() -> bool;
    auto SetDirty() -> void;
    auto ClearDirty() -> void;
  };
}  // namespace EditorGUI