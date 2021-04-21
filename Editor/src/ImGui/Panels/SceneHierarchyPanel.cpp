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

    if (ImGui::Button("+", ImVec2{25, 25})) {
      ImGui::OpenPopup("Create");
    }

    DrawEntity(ECS::EntityManager::GetInstance().GetEntity(0));

    if (ImGui::BeginPopup("Create")) {
      if (ImGui::MenuItem("Create Empty")) {
        auto ent = ECS::EntityManager::GetInstance().CreateEntity();
        ent->AddComponent< Transform >();
        ent->Name("New Entity");
        m_Scene->SceneGraph()->AddChild(0, ent->GetID());
        ImGui::CloseCurrentPopup();
      }
      ImGui::EndPopup();
    }
    ImGui::End();
  }

  void SceneHierarchyPanel::DrawEntity(std::shared_ptr< ECS::Entity > entity) {
    auto id = entity->GetID();
    auto tag             = entity->Name();
    const auto& children = m_Scene->SceneGraph()->GetChildren(id);

    ImGuiTreeNodeFlags flags = ((m_SelectedEntity == entity) ? ImGuiTreeNodeFlags_Selected : 0)
                               | ((children.size() == 0) ? ImGuiTreeNodeFlags_Leaf : 0)
                               | ((id == 0) ? ImGuiTreeNodeFlags_DefaultOpen : 0)
                               | ImGuiTreeNodeFlags_OpenOnArrow;
    
    bool open = ImGui::TreeNodeEx((void*)entity->GetID(), flags, tag.c_str());
    if (ImGui::IsItemClicked()) {
      m_SelectedEntity = entity;
    }
    if (ImGui::BeginDragDropSource()) {
      ImGui::SetDragDropPayload("Scene_Hierarchy", &id, sizeof(ECS::EntityID));
      ImGui::EndDragDropSource();
    }
    if (ImGui::BeginDragDropTarget()) {
      if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Scene_Hierarchy")) {
        IM_ASSERT(payload->DataSize == sizeof(ECS::EntityID));
        ECS::EntityID payload_n = *(const ECS::EntityID*)payload->Data;
        m_Scene->SceneGraph()->SetParent(payload_n, id);
      }
      ImGui::EndDragDropTarget();
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