#pragma once
#include "Engine.h"
#include <filesystem>

class EditorLayer;

namespace Editor {
  class MaterialPanel {
  private:
    EditorLayer* _editorLayer;
    char _charBuffer[100]{0};
    bool _isDirty{false};
    std::shared_ptr< Engine::Renderer::Material > _material;
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
    auto MaterialSelection() -> void;
    auto DrawMaterialView() -> void;

    auto ShaderSelection() -> void;
    auto TextureSelection() -> void;
    auto PBRParams() -> void;
    auto ColorSelection() -> void;
    auto StencilSetup() -> void;

    auto SaveMaterial(const std::shared_ptr< Engine::Renderer::Material >& material) -> void;
    auto LoadMaterial(const std::string& file) -> void;
    // Not Working
    auto ShowSaveModal(const std::string& loadFile) -> void;
    auto IsDirty() -> bool;
    auto SetDirty() -> void;
    auto ClearDirty() -> void;
  };
}  // namespace Editor