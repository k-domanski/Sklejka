#include "SceneHierarchyPanel.h"
#include <imgui/imgui.h>

namespace EditorGUI {
  SceneHierarchyPanel::SceneHierarchyPanel(std::shared_ptr< Scene > scene): m_Scene(scene) {
  }

  void SceneHierarchyPanel::SetScene(std::shared_ptr< Scene > scene) {
    m_Scene = scene;
  }

  void SceneHierarchyPanel::OnImGuiRender() {
    ImGui::Begin("Scene Hierarchy");
    auto entities = m_Scene->SceneGraph()->GetChildren(m_Scene->SceneGraph()->GetRootID());
    for (auto& entity : entities) {
      DrawEntity(entity);
    }
    ImGui::End();
  }

  void SceneHierarchyPanel::DrawEntity(ECS::EntityID entity) {
    std::string tag          = std::to_string(entity);
    ImGuiTreeNodeFlags flags = ((m_SelectedEntity == entity) ? ImGuiTreeNodeFlags_Selected : 0)
                               | ImGuiTreeNodeFlags_OpenOnArrow;

    bool open = ImGui::TreeNodeEx((void*)entity, flags, tag.c_str());
    if (open) {
      auto children = m_Scene->SceneGraph()->GetChildren(entity);
      for (auto& child : children) {
        DrawEntity(child);
      }
      ImGui::TreePop();
    }
  }
  void SceneHierarchyPanel::SetSelectedEntity(ECS::EntityID entity) {
    m_SelectedEntity = entity;
  }
}  // namespace EditorGUI