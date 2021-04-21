#include "InspectorPanel.h"
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

namespace EditorGUI {
  void InspectorPanel::OnImGuiRender(std::shared_ptr< ECS::Entity > entity) {
    ImGui::Begin("Inspector");
    if (entity)
      DrawComponents(entity);
    ImGui::End();
  }

  template< class T, typename UIFunction >
  static void DrawComponent(const std::string& name, std::shared_ptr< ECS::Entity > entity,
                            UIFunction function) {
    const ImGuiTreeNodeFlags treeNodeFlags =
        ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed
        | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap
        | ImGuiTreeNodeFlags_FramePadding;

    if (entity->GetComponent< T >() != nullptr) {
      auto& component               = entity->GetComponent< T >();
      ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

      ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{10, 10});
      float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
      ImGui::Separator();
      bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name.c_str());
      ImGui::PopStyleVar();
      ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);

      /* TODO: Remove component in pop up menu, some other settings*/
      if (ImGui::Button("+", ImVec2{lineHeight, lineHeight})) {
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
        function(component);
        ImGui::TreePop();
      }

      if (removeComponent) {
        /*TODO:: Entity->RemoveComponent<>();*/
        APP_DEBUG("Removed {}", component->Name());
      }
    }
  }

  /*-------Checkbox bools----------*/
  static bool activeCamera;
  // static bool boxTrigger;
  // static bool sphereTrigger;
  // static bool rbGravity;
  // static bool rbKinematic;
  // static bool sphere;
  /*-------------------------------*/
  void InspectorPanel::DrawComponents(std::shared_ptr< ECS::Entity > entity) {
    /*Name Input Field*/
    auto tag = entity->Name();

    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    std::strncpy(buffer, tag.c_str(), sizeof(buffer));
    if (ImGui::InputText("##Name", buffer, sizeof(buffer))) {
      entity->Name(std::string(buffer));
    }

    ImGui::SameLine();
    ImGui::PushItemWidth(-1);

    if (ImGui::Button("Add Component")) {
      ImGui::OpenPopup("Add Component");
    }

    if (ImGui::BeginPopup("Add Component")) {
      if (ImGui::MenuItem("Mesh Renderer")) {
        if (entity->GetComponent< Components::MeshRenderer >() == nullptr) {
          entity->AddComponent< Components::MeshRenderer >();
        } else {
          APP_WARN("{} entity already has Mesh Renderer component!", entity->Name());
        }
        ImGui::CloseCurrentPopup();
      }

      if (ImGui::MenuItem("Camera")) {
        if (entity->GetComponent< Camera >() == nullptr) {
          /*Temporary TODO: default constructor?*/
          entity->AddComponent< Camera >(45.0f, 1.78, 0.001f, 1000.0f);
        } else {
          APP_WARN("{} entity already has Camera component!", entity->Name());
        }
        ImGui::CloseCurrentPopup();
      }

      if (ImGui::MenuItem("Rigidbody")) {
        if (entity->GetComponent< Components::Rigidbody >() == nullptr) {
          entity->AddComponent< Components::Rigidbody >();
        } else {
          APP_WARN("{} entity already has Rigidbody component!", entity->Name());
        }
        ImGui::CloseCurrentPopup();
      }

      if (ImGui::MenuItem("Collider")) {
        if (entity->GetComponent< Components::Collider >() == nullptr) {
          entity->AddComponent< Components::Collider >();
        } else {
          APP_WARN("{} entity already has Collider component!", entity->Name());
        }
        ImGui::CloseCurrentPopup();
      }

      /*if (ImGui::MenuItem("Sphere Collider")) {
        if (entity->GetComponent< Components::Collider >() == nullptr) {
          entity->AddComponent< Components::Collider >();
        } else {
          APP_WARN("{} entity already has Sphere Collider component!", entity->Name());
        }
        ImGui::CloseCurrentPopup();
      }*/
      ImGui::EndPopup();
    }
    DrawComponent< Transform >("Transform", entity, [](auto component) {
      /*Position*/
      glm::vec3 pos = component->Position();
      DrawVec3("Translation", pos);
      component->Position(pos);
      /*Rotation*/
      glm::vec3 rotation = glm::degrees(component->Euler());
      DrawVec3("Rotation", rotation);
      component->Rotation(component->Euler(glm::radians(rotation)));
      /*Scale*/
      glm::vec3 scale = component->Scale();
      DrawVec3("Scale", scale, 1.0f);
      component->Scale(scale);
    });

    DrawComponent< Camera >("Camera", entity, [](auto component) {
      /*Przelaczanie flagi MainCamera*/
      if (component->flags.Get(CameraFlag::MainCamera))
        activeCamera = true;
      DrawBool("Main Camera", activeCamera);
      if (!activeCamera)
        component->flags.Clear(CameraFlag::MainCamera);
      else
        component->flags.Set(CameraFlag::MainCamera);

      // APP_DEBUG("{}", component->flags.GetState());
      /*FOV*/
      float fov = component->Fov();
      DrawFloat("FOV", fov);
      component->Fov(fov);
      /*Aspect*/
      float aspect = component->Aspect();
      DrawFloat("Aspect", aspect);
      component->Aspect(aspect);
      /*Near plane*/
      float nearPlane = component->NearPlane();
      DrawFloat("Near plane", nearPlane);
      component->NearPlane(nearPlane);
      /*Far plane*/
      float farPlane = component->FarPlane();
      DrawFloat("Far plane", farPlane);
      component->FarPlane(farPlane);
    });

    DrawComponent< Components::MeshRenderer >("Mesh Renderer", entity, [](auto component) {
      /*
          mesh
          material
      */
    });

    DrawComponent< Components::Rigidbody >("Rigidbody", entity, [](auto component) {
      glm::vec3 velocity = component->GetVelocity();
      DrawVec3("Velocity", velocity);
      component->SetVelocity(velocity);

      bool rbKinematic = component->IsKinematic();
      bool rbGravity = component->UseGravity();

      //if (component->UseGravity())
        //rbGravity = true;
      DrawBool("Use Gravity", rbGravity);
      component->SetGravity(rbGravity);

      //if (component->IsKinematic())
        //rbKinematic = true;
      DrawBool("Is Kinematic", rbKinematic);
      component->SetKinematic(rbKinematic);
    });

    DrawComponent< Components::Collider >("Collider", entity, [](auto component) {
      bool boxTrigger;
      bool sphere;
      glm::vec3 size = component->Size;
      DrawVec3("Size", size);
      component->Size  = size;
      glm::vec3 center = component->Center;
      DrawVec3("Center", center);
      component->Center = center;

      if (component->Type == +Components::ColliderType::Sphere)
        sphere = true;
      else
        sphere = false;

      // if (component->IsTrigger)
      boxTrigger = component->IsTrigger;

      DrawBool("Is sphere", sphere);
      if (sphere)
        component->Type = Engine::Components::ColliderType::Sphere;
      else
        component->Type = Engine::Components::ColliderType::Box;

      DrawBool("Is Trigger", boxTrigger);
      component->IsTrigger = boxTrigger;
    });

    /*DrawComponent< Components::Collider >("Sphere Collider", entity, [](auto component) {
      float radius = component->Size.x;
      DrawFloat("Radius", radius);
      component->Size = glm::vec3(radius);

      if (component->IsTrigger)
        boxTrigger = true;
      DrawBool("Is Trigger", boxTrigger);
      component->IsTrigger = boxTrigger;
    });*/
  }

  static void DrawVec3(const std::string& name, glm::vec3& value, float resetValue = 0.0f) {
    float columnWidth = 100.0f;
    ImGuiIO& io       = ImGui::GetIO();

    auto boldFont = io.Fonts->Fonts[0];

    ImGui::PushID(name.c_str());

    ImGui::Columns(2);
    ImGui::SetColumnWidth(0, columnWidth);
    ImGui::Text(name.c_str());
    ImGui::NextColumn();

    ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{2, 2});

    float lineHeight  = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
    ImVec2 buttonSize = {lineHeight + 3.0f, lineHeight};

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.8f, 0.1f, 0.15f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.9f, 0.2f, 0.2f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.8f, 0.1f, 0.15f, 1.0f});
    ImGui::PushFont(boldFont);
    if (ImGui::Button("X", buttonSize))
      value.x = resetValue;
    ImGui::PopFont();
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::DragFloat("##X", &value.x, 0.1f, 0.0f, 0.0f, "%.2f");
    ImGui::PopItemWidth();
    ImGui::SameLine();

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.2f, 0.7f, 0.2f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.3f, 0.8f, 0.3f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.2f, 0.7f, 0.2f, 1.0f});
    ImGui::PushFont(boldFont);
    if (ImGui::Button("Y", buttonSize))
      value.y = resetValue;
    ImGui::PopFont();
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::DragFloat("##Y", &value.y, 0.1f, 0.0f, 0.0f, "%.2f");
    ImGui::PopItemWidth();
    ImGui::SameLine();

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.1f, 0.25f, 0.8f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.2f, 0.35f, 0.9f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.1f, 0.25f, 0.8f, 1.0f});
    ImGui::PushFont(boldFont);
    if (ImGui::Button("Z", buttonSize))
      value.z = resetValue;
    ImGui::PopFont();
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::DragFloat("##Z", &value.z, 0.1f, 0.0f, 0.0f, "%.2f");
    ImGui::PopItemWidth();

    ImGui::PopStyleVar();

    ImGui::Columns(1);

    ImGui::PopID();
  }

  static void DrawFloat(const std::string& name, float& value, float sliderSpeed = 0.01f) {
    float columnWidth = 100.0f;
    ImGuiIO& io       = ImGui::GetIO();

    ImGui::PushID(name.c_str());

    ImGui::Columns(2);
    ImGui::SetColumnWidth(0, columnWidth);
    ImGui::Text(name.c_str());
    ImGui::NextColumn();

    ImGui::PushMultiItemsWidths(1, ImGui::CalcItemWidth());
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{2, 2});

    ImGui::DragFloat("##X", &value, sliderSpeed, 0.0f, 0.0f, "%.2f");
    ImGui::PopItemWidth();
    ImGui::PopStyleVar();
    ImGui::Columns(1);
    ImGui::PopID();
  }
  static void DrawBool(const std::string& name, bool& value) {
    float columnWidth = 100.0f;
    ImGuiIO& io       = ImGui::GetIO();

    ImGui::PushID(name.c_str());
    ImGui::Columns(2);
    ImGui::SetColumnWidth(0, columnWidth);
    ImGui::Text(name.c_str());
    ImGui::NextColumn();

    ImGui::PushMultiItemsWidths(1, ImGui::CalcItemWidth());
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{2, 2});
    ImGui::Checkbox("", &value);

    ImGui::PopItemWidth();
    ImGui::PopStyleVar();
    ImGui::Columns(1);
    ImGui::PopID();
  }
}  // namespace EditorGUI