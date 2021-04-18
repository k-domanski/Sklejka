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
      DrawEntity(ECS::EntityManager::GetInstance().GetEntity(entity));
    }
    ImGui::End();
  }

  void SceneHierarchyPanel::DrawEntity(std::shared_ptr<ECS::Entity> entity) {
    ECS::EntityID id = entity->GetID();
    std::string tag          = std::to_string(id);
    ImGuiTreeNodeFlags flags = ((m_SelectedEntity == entity) ? ImGuiTreeNodeFlags_Selected : 0)
                               | ImGuiTreeNodeFlags_OpenOnArrow;

    bool open = ImGui::TreeNodeEx((void*)entity->GetID(), flags, tag.c_str());
    if (open) {
      auto children = m_Scene->SceneGraph()->GetChildren(id);
      for (auto& child : children) {
        DrawEntity(ECS::EntityManager::GetInstance().GetEntity(child));
      }
      ImGui::TreePop();
    }
    if (ImGui::IsItemClicked())
        m_SelectedEntity = entity;
  }
  void SceneHierarchyPanel::SetSelectedEntity(std::shared_ptr<ECS::Entity> entity) {
    m_SelectedEntity = entity;
  }
}  // namespace EditorGUI