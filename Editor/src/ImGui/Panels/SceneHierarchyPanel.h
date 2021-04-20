#pragma once
#include "Engine.h"

namespace EditorGUI {
  class SceneHierarchyPanel {
  public:
    SceneHierarchyPanel() = default;
    SceneHierarchyPanel(std::shared_ptr< Engine::Scene > scene);

    std::shared_ptr< Engine::ECS::Entity > GetSelectedEntity() {
      return m_SelectedEntity;
    }
    void SetScene(std::shared_ptr< Engine::Scene > scene);
    void OnImGuiRender();
    void SetSelectedEntity(std::shared_ptr< Engine::ECS::Entity > entity);

  private:
    void DrawEntity(std::shared_ptr< Engine::ECS::Entity > entity);

  private:
    std::shared_ptr< Engine::Scene > m_Scene;
    std::shared_ptr< Engine::ECS::Entity > m_SelectedEntity;
  };
}  // namespace EditorGUI