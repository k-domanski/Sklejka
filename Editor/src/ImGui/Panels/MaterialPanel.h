#pragma once
#include "Engine.h"
#include <filesystem>

class EditorLayer;

namespace EditorGUI {
  class MaterialPanel {
  private:
    EditorLayer* _editorLayer;
    std::string _strBuffer;
    std::shared_ptr< Engine::Renderer::Material > _selectedMaterial;
    std::filesystem::path _materialsFolder;

  public:
    MaterialPanel();
    auto OnImGuiRender() -> void;
    auto SetEditorLayer(EditorLayer* layer) -> void;
    auto SetMaterial(const std::shared_ptr< Engine::Renderer::Material >& material) -> void;

  private:
    auto DrawMaterialView() -> void;
  };
}  // namespace EditorGUI