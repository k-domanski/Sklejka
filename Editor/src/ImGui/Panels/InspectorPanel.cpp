#include "InspectorPanel.h"
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

namespace Editor {

  void InspectorPanel::OnImGuiRender() {
    ImGui::Begin("Inspector");
    if (_entity != nullptr) {
      ShowComponentsDropdown();
      for (auto& view : _componentViews) {
        WrapViewInTreeNode(view);
      }
    }
    ImGui::End();
  }

  auto InspectorPanel::AttachEntity(const std::shared_ptr< ECS::Entity >& entity) -> void {
    _entity = entity;
    PopulateComponentViews();
    for (auto& view : _componentViews) {
      view->AttachEntity(entity);
    }
  }

  auto InspectorPanel::ReattachEntity() -> void {
    PopulateComponentViews();
    for (auto& view : _componentViews) {
      view->AttachEntity(_entity);
    }
  }

  auto InspectorPanel::PopulateComponentViews() -> void {
    _componentViews.clear();
    if (_entity->GetID() == 0)
      return;
    if (_entity->HasComponent< Engine::Transform >()) {
      _componentViews.push_back(std::make_shared< TransformView >());
    }
    if (_entity->HasComponent< Engine::Camera >()) {
      _componentViews.push_back(std::make_shared< CameraView >());
    }
    if (_entity->HasComponent< Engine::Components::MeshRenderer >()) {
      _componentViews.push_back(std::make_shared< MeshRendererView >());
    }
    if (_entity->HasComponent< Engine::Components::Rigidbody >()) {
      _componentViews.push_back(std::make_shared< RigidbodyView >());
    }
    if (_entity->HasComponent< Engine::Components::Collider >()) {
      _componentViews.push_back(std::make_shared< ColliderView >());
    }
    if (_entity->HasComponent< Engine::DirectionalLight >()) {
      _componentViews.push_back(std::make_shared< DirectionalLightView >());
    }
    if (_entity->HasComponent< Engine::Node >()) {
      _componentViews.push_back(std::make_shared< NodeView >());
    }
  }

  auto InspectorPanel::WrapViewInTreeNode(const std::shared_ptr< IComponentView >& view) -> void {
    const ImGuiTreeNodeFlags treeNodeFlags =
        ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed
        | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap
        | ImGuiTreeNodeFlags_FramePadding;

    ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{10, 10});
    float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
    ImGui::Separator();
    auto id = std::hash< std::string >{}(view->Component()->Name());
    bool open =
        ImGui::TreeNodeEx((const void*)id, treeNodeFlags, view->Component()->Name().c_str());
    ImGui::PopStyleVar();
    ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);

    /* TODO: Remove component in pop up menu, some other settings*/
    if (ImGui::Button("...", ImVec2{lineHeight, lineHeight})) {
      ImGui::OpenPopup("ComponentSettings");
    }

    bool removeComponent = false;
    if (ImGui::BeginPopup("ComponentSettings")) {
      if (ImGui::MenuItem("Remove Component")) {
        removeComponent = true;
      }
      ImGui::EndPopup();
    }
    /*-----------------------------------------------------------*/

    if (open) {
      view->OnDraw();
      ImGui::TreePop();
    }

    if (removeComponent) {
      /*TODO:: Entity->RemoveComponent<>();*/
      LOG_DEBUG("Removed {}", view->Component()->Name());
    }
  }

  auto InspectorPanel::ShowComponentsDropdown() -> void {
    auto tag = _entity->Name();
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    std::strncpy(buffer, tag.c_str(), sizeof(buffer));
    if (ImGui::InputText("##Name", buffer, sizeof(buffer))) {
      _entity->Name(std::string(buffer));
    }
    /*No components for root*/
    if (_entity->GetID() == 0)
      return;

    ImGui::SameLine();
    ImGui::PushItemWidth(-1);

    if (ImGui::Button("Add Component")) {
      ImGui::OpenPopup("Add Component");
    }

    if (ImGui::BeginPopup("Add Component")) {
      if (ImGui::MenuItem("Mesh Renderer")) {
        if (_entity->GetComponent< Components::MeshRenderer >() == nullptr) {
          _entity->AddComponent< Components::MeshRenderer >();
          ReattachEntity();
        } else {
          APP_WARN("{} entity already has Mesh Renderer component!", _entity->Name());
        }
        ImGui::CloseCurrentPopup();
      }

      if (ImGui::MenuItem("Camera")) {
        if (_entity->GetComponent< Camera >() == nullptr) {
          /*Temporary TODO: default constructor?*/
          _entity->AddComponent< Camera >(45.0f, 1.78, 0.001f, 1000.0f);
          ReattachEntity();
        } else {
          APP_WARN("{} entity already has Camera component!", _entity->Name());
        }
        ImGui::CloseCurrentPopup();
      }

      if (ImGui::MenuItem("Rigidbody")) {
        if (_entity->GetComponent< Components::Rigidbody >() == nullptr) {
          _entity->AddComponent< Components::Rigidbody >();
          ReattachEntity();
        } else {
          APP_WARN("{} entity already has Rigidbody component!", _entity->Name());
        }
        ImGui::CloseCurrentPopup();
      }

      if (ImGui::MenuItem("Collider")) {
        if (_entity->GetComponent< Components::Collider >() == nullptr) {
          _entity->AddComponent< Components::Collider >();
          ReattachEntity();
        } else {
          APP_WARN("{} entity already has Collider component!", _entity->Name());
        }
        ImGui::CloseCurrentPopup();
      }
      if (ImGui::MenuItem("Directional Light")) {
        if (_entity->GetComponent< DirectionalLight >() == nullptr) {
          _entity->AddComponent< DirectionalLight >();
          ReattachEntity();
        } else {
          APP_WARN("{} entity already has DirectionalLight component!", _entity->Name());
        }
        ImGui::CloseCurrentPopup();
      }
      if (ImGui::MenuItem("Node")) {
        if (_entity->GetComponent< Node >() == nullptr) {
          _entity->AddComponent< Node >();
          ReattachEntity();
        } else {
          APP_WARN("{} entity already has Node component!", _entity->Name());
        }
        ImGui::CloseCurrentPopup();
      }
      ReattachEntity();
      ImGui::EndPopup();
    }
  }
}  // namespace Editor