#pragma once
#include "Engine.h"

using namespace Engine;

namespace EditorGUI {
  class SceneHierarchyPanel {
  public:
    SceneHierarchyPanel() = default;
    SceneHierarchyPanel(std::shared_ptr< Scene > scene);

    std::shared_ptr<ECS::Entity> GetSelectedEntity() { return m_SelectedEntity; }
    void SetScene(std::shared_ptr< Scene > scene);
    void OnImGuiRender();

  private:
    void DrawEntity(std::shared_ptr<ECS::Entity> entity);
    void SetSelectedEntity(std::shared_ptr<ECS::Entity> entity);

  private:
    std::shared_ptr< Scene > m_Scene;
    std::shared_ptr<ECS::Entity> m_SelectedEntity;
  };
}  // namespace EditorGUI