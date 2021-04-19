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

      if (ImGui::BeginPopup("ComponentSettings")) {
        if (ImGui::MenuItem("test")) {}
        ImGui::EndPopup();
      }
      /*-----------------------------------------------------------*/

      if (open) {
        function(component);
        ImGui::TreePop();
      }
    }
  }

  /*-------Checkbox bools----------*/
  static bool activeCamera;
  static bool boxTrigger;
  static bool sphereTrigger;
  static bool rbGravity;
  static bool rbKinematic;
  /*-------------------------------*/
  void InspectorPanel::DrawComponents(std::shared_ptr< ECS::Entity > entity) {
      /*Name Input Field*/
      auto tag = entity->Name();

      char buffer[256];
      memset(buffer, 0, sizeof(buffer));
      std::strncpy(buffer, tag.c_str(), sizeof(buffer));
      if (ImGui::InputText("##Name", buffer, sizeof(buffer)))
      {
          entity->Name(std::string(buffer));
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

      if (component->UseGravity())
        rbGravity = true;
      DrawBool("Use Gravity", rbGravity);
      component->SetGravity(rbGravity);

      if (component->IsKinematic())
        rbKinematic = true;
      DrawBool("Is Kinematic", rbKinematic);
      component->SetKinematic(rbKinematic);
    });

    DrawComponent< Components::BoxCollider >("Box Collider", entity, [](auto component) {
      glm::vec3 size = component->GetSize();
      DrawVec3("Size", size);
      component->SetSize(size);

      if (component->IsTrigger())
        boxTrigger = true;
      DrawBool("Is Trigger", boxTrigger);
      component->SetTrigger(boxTrigger);
    });

    DrawComponent< Components::SphereCollider >("Sphere Collider", entity, [](auto component) {
      float radius = component->GetRadius();
      DrawFloat("Radius", radius);
      component->SetRadius(radius);

      if (component->IsTrigger())
        boxTrigger = true;
      DrawBool("Is Trigger", boxTrigger);
      component->SetTrigger(boxTrigger);
    });
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