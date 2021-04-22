#pragma once
#include "Engine.h"

class EditorLayer;
namespace Editor {
  class SceneHierarchyPanel {
  public:
    SceneHierarchyPanel();
    SceneHierarchyPanel(std::shared_ptr< Engine::Scene > scene);

    std::shared_ptr< Engine::ECS::Entity > GetSelectedEntity() {
      return m_SelectedEntity;
    }
    auto SetEditorLayer(EditorLayer* layer) -> void {
      _editorLayer = layer;
    }
    void SetSelectedEntity(const std::shared_ptr< Engine::ECS::Entity >& entity) {
      m_SelectedEntity = entity;
      _selectedCallback(entity);
    }
    void SetSelectionCallback(
        const std::function< void(const std::shared_ptr< Engine::ECS::Entity >) >& callback) {
      _selectedCallback = callback;
    }
    void SetScene(std::shared_ptr< Engine::Scene > scene);
    void OnImGuiRender();

  private:
    void DrawEntity(std::shared_ptr< Engine::ECS::Entity > entity);

  private:
    EditorLayer* _editorLayer;
    std::shared_ptr< Engine::Scene > m_Scene;
    std::shared_ptr< Engine::ECS::Entity > m_SelectedEntity;
    std::function< void(const std::shared_ptr< Engine::ECS::Entity >) > _selectedCallback;
  };
}  // namespace Editor