#pragma once
#include "Engine.h"
#include "ImGui/ImGui/ImGuiLayer.h"

class EditorLayer;
namespace Editor {
  class NodesUtilityPanel {
  private:
    std::shared_ptr< Engine::Scene > _scene = nullptr;
    EditorLayer* _editorLayer;

  public:
    NodesUtilityPanel() = default;
    auto SetEditorLayer(EditorLayer* layer) -> void;
    auto SetScene(const std::shared_ptr< Engine::Scene >& scene) -> void;
    auto OnImGuiRender() -> void;
  };
}  // namespace Editor