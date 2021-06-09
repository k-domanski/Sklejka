#pragma once
#include <Engine.h>
#include "MeshMapping.h"

using namespace Engine;
using namespace Engine::ECS;
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

  private:
    auto AddColliders(const std::vector< Editor::TransformData >& collider_mapping,
                      std::vector< std::shared_ptr< Entity > > entities) -> void;
    auto ProcessBell(std::shared_ptr< Entity >& entity) -> void;
  };

}  // namespace Editor