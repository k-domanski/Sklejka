#pragma once
#include "IComponentView.h"
#include <Engine.h>
#include "AttributeView.h"

namespace Editor {
  /* Base Class*/
  template< typename T >
  class ComponentView : public IComponentView {
  protected:
    std::shared_ptr< T > _component{nullptr};
    std::shared_ptr< Engine::ECS::Entity > _entity{nullptr};

  public:
    ComponentView() = default;
    auto AttachEntity(const std::shared_ptr< Engine::ECS::Entity >& entity) -> void override {
      _entity    = entity;
      _component = entity->GetComponent< T >();
    }
    auto Component() const -> std::shared_ptr< Engine::ECS::Component > override {
      return _component;
    }
    virtual auto OnDraw() -> void override = 0;
  };

  /* Per Component Views */
  /* Transform */
  class TransformView : public ComponentView< Engine::Transform > {
  private:
    glm::vec3 _rotationWrite;

  public:
    auto AttachEntity(const std::shared_ptr< Engine::ECS::Entity >& entity) -> void override {
      ComponentView::AttachEntity(entity);
      _rotationWrite = glm::degrees(_component->Euler());
    }
    auto OnDraw() -> void override {
      glm::vec3 pos = _component->Position();
      DrawVec3("Translation", pos);
      _component->Position(pos);
      /*Rotation*/
      DrawVec3("Rotation", _rotationWrite);
      _component->Euler(glm::radians(_rotationWrite));
      /*Scale*/
      glm::vec3 scale = _component->Scale();
      DrawVec3("Scale", scale, 1.0f);
      _component->Scale(scale);
    }
  };
  /* Camera */
  class CameraView : public ComponentView< Engine::Camera > {
  public:
    auto OnDraw() -> void override {
      /*Przelaczanie flagi MainCamera*/
      /* YYY nie */
      auto active = _component->flags.GetAll(Engine::CameraFlag::MainCamera);
      DrawBool("Main Camera", active);
      if (!active)
        _component->flags.Clear(Engine::CameraFlag::MainCamera);
      else
        _component->flags.Set(Engine::CameraFlag::MainCamera);

      // APP_DEBUG("{}", _component->flags.GetState());
      /*FOV*/
      float fov = _component->Fov();
      DrawFloat("FOV", fov);
      _component->Fov(fov);
      /*Aspect*/
      float aspect = _component->Aspect();
      DrawFloat("Aspect", aspect);
      _component->Aspect(aspect);
      /*Near plane*/
      float nearPlane = _component->NearPlane();
      DrawFloat("Near plane", nearPlane);
      _component->NearPlane(nearPlane);
      /*Far plane*/
      float farPlane = _component->FarPlane();
      DrawFloat("Far plane", farPlane);
      _component->FarPlane(farPlane);
    }
  };
  /* Mesh Renderer */
  class MeshRendererView : public ComponentView< Engine::Components::MeshRenderer > {
  private:
      std::string name;
  public:
    auto OnDraw() -> void override {
        std::string model = "Model";
        bool hasMaterial = _component->GetMaterial() != nullptr;
        name = hasMaterial ? _component->GetMaterial()->FilePath() : "<NONE>";
        ImGui::PushID(model.c_str());

        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, 100);
        ImGui::Text(model.c_str());
        ImGui::NextColumn();
        //ImGui::Text("Kolumna druga");
        ImGui::Columns(1);
        //ImGui::PopItemWidth();
        ImGui::PopID();
        if (ImGui::BeginChild("File", ImVec2{ 0,/*ImGui::CalcItemWidth()*/0 }))
        {
            ImGui::Text(name.c_str());
        }
        ImGui::EndChild();
        if (ImGui::BeginDragDropTarget()) {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("FILE")) {
                auto payload_str = std::string(static_cast<char*>(payload->Data));
                auto material = Engine::AssetManager::CreateMaterial(payload_str);
                _component->SetMaterial(material);
            }
            ImGui::EndDragDropTarget();
        }
        ImGui::Separator();

        std::string mat = "Material";
        ImGui::PushID(mat.c_str());

        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, 100);
        ImGui::Text(mat.c_str());
        ImGui::NextColumn();

        ImGui::Columns(1);
        ImGui::PopID();
    }
  };
  /* Rigidbody */
  class RigidbodyView : public ComponentView< Engine::Components::Rigidbody > {
  public:
    auto OnDraw() -> void override {
      glm::vec3 velocity = _component->GetVelocity();
      DrawVec3("Velocity", velocity);
      _component->SetVelocity(velocity);

      bool rbKinematic = _component->IsKinematic();
      bool rbGravity   = _component->UseGravity();

      DrawBool("Use Gravity", rbGravity);
      _component->SetGravity(rbGravity);

      DrawBool("Is Kinematic", rbKinematic);
      _component->SetKinematic(rbKinematic);
    }
  };
  /* Collider */
  class ColliderView : public ComponentView< Engine::Components::Collider > {
  public:
    auto OnDraw() -> void override {
      bool boxTrigger;
      bool sphere;
      glm::vec3 size = _component->Size;
      DrawVec3("Size", size);
      _component->Size = size;
      glm::vec3 center = _component->Center;
      DrawVec3("Center", center);
      _component->Center = center;

      if (_component->Type == +Engine::Components::ColliderType::Sphere)
        sphere = true;
      else
        sphere = false;

      boxTrigger = _component->IsTrigger;

      DrawBool("Is sphere", sphere);
      if (sphere)
        _component->Type = +Engine::Components::ColliderType::Sphere;
      else
        _component->Type = +Engine::Components::ColliderType::Box;

      DrawBool("Is Trigger", boxTrigger);
      _component->IsTrigger = boxTrigger;
    }
  };
}  // namespace Editor