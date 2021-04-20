#include "SceneHierarchyPanel.h"
#include <imgui/imgui.h>

using namespace Engine;
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

  void SceneHierarchyPanel::DrawEntity(std::shared_ptr< ECS::Entity > entity) {
    auto id              = entity->GetID();
    auto tag             = entity->Name();
    const auto& children = m_Scene->SceneGraph()->GetChildren(id);

    ImGuiTreeNodeFlags flags = ((m_SelectedEntity == entity) ? ImGuiTreeNodeFlags_Selected : 0)
                               | ((children.size() == 0) ? ImGuiTreeNodeFlags_Leaf : 0)
                               | ImGuiTreeNodeFlags_OpenOnArrow;

    bool open = ImGui::TreeNodeEx((void*)entity->GetID(), flags, tag.c_str());
    if (ImGui::IsItemClicked()) {
      m_SelectedEntity = entity;
    }
    if (open) {
      for (auto& child : children) {
        DrawEntity(ECS::EntityManager::GetInstance().GetEntity(child));
      }
      ImGui::TreePop();
    }
  }
  void SceneHierarchyPanel::SetSelectedEntity(std::shared_ptr< ECS::Entity > entity) {
    m_SelectedEntity = entity;
  }
}  // namespace EditorGUI