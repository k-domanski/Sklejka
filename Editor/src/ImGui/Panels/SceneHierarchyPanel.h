#pragma once
#include "Engine.h"

using namespace Engine;

namespace EditorGUI {
  class SceneHierarchyPanel {
  public:
    SceneHierarchyPanel() = default;
    SceneHierarchyPanel(std::shared_ptr< Scene > scene);

    void SetScene(std::shared_ptr< Scene > scene);
    void OnImGuiRender();

  private:
    void DrawEntity(ECS::EntityID entity);
    void SetSelectedEntity(ECS::EntityID entity);

  private:
    std::shared_ptr< Scene > m_Scene;
    ECS::EntityID m_SelectedEntity;
  };
}  // namespace EditorGUI