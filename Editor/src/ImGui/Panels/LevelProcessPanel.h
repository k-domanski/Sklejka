#pragma once
#include <Engine.h>

class EditorLayer;
namespace Editor {
  class LevelProcessPanel {
  private:
    EditorLayer* _editorLayer;
    std::shared_ptr< Engine::Scene > _scene;

  public:
    LevelProcessPanel();
    auto SetScene(const std::shared_ptr< Engine::Scene >& scene) -> void;
    auto SetEditorLayer(EditorLayer* layer) -> void;
    auto OnImGuiRender() -> void;
  };

}  // namespace Editor