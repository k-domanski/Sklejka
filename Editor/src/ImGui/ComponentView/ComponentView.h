#pragma once
#include "IComponentView.h"
#include <Engine.h>
#include "AttributeView.h"
#include "Components/Collider.h"
#include "Components/Rigidbody.h"
#include <filesystem>

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
    std::string modelPath;
    std::string materialPath;
    std::filesystem::path _materialsFolder =
        std::filesystem::current_path().string() + "\\materials\\";
    std::filesystem::path _modelsFolder = std::filesystem::current_path().string() + "\\models\\";

  public:
    auto OnDraw() -> void override {
      bool hasModel    = _component->GetModel() != nullptr;
      bool hasMaterial = _component->GetMaterial() != nullptr;
      modelPath =
          hasModel
              ? std::filesystem::path(_component->GetModel()->GetFilepath()).filename().string()
              : "<None>";
      materialPath =
          hasMaterial
              ? std::filesystem::path(_component->GetMaterial()->FilePath()).filename().string()
              : "<None>";
      ImGui::PushID("Model");

      ImGui::Columns(2);
      ImGui::SetColumnWidth(0, 100);
      ImGui::Text("Model");
      ImGui::NextColumn();
      if (ImGui::BeginCombo("", modelPath.c_str())) {
        for (auto& entry : std::filesystem::recursive_directory_iterator(_modelsFolder)) {
          const auto path     = entry.path();
          const auto filename = path.filename().string();
          const auto ext      = path.extension();
          if (ext != ".fbx")
            continue;
          if (ImGui::Selectable(filename.c_str())) {
            auto model = Engine::AssetManager::GetModel(path.string());
            _component->SetModel(model);
          }
        }
        ImGui::EndCombo();
      }
      if (ImGui::BeginDragDropTarget()) {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("FILE")) {
          auto payload_str = std::string(static_cast< char* >(payload->Data));
          LoadModel(payload_str);
        }
        ImGui::EndDragDropTarget();
      }
      ImGui::Columns(1);
      ImGui::PopID();
      ImGui::Separator();

      std::string mat = "Material";
      ImGui::PushID(mat.c_str());
      ImGui::Columns(2);
      ImGui::SetColumnWidth(0, 100);
      ImGui::Text(mat.c_str());
      ImGui::NextColumn();
      if (ImGui::BeginCombo("", materialPath.c_str())) {
        for (auto& entry : std::filesystem::recursive_directory_iterator(_materialsFolder)) {
          const auto path     = entry.path();
          const auto filename = path.filename().string();
          const auto ext      = path.extension();
          if (ext != ".mat")
            continue;
          if (ImGui::Selectable(filename.c_str())) {
            auto material = Engine::AssetManager::GetMaterial(path.string());
            _component->SetMaterial(material);
          }
        }
        ImGui::EndCombo();
      }
      if (ImGui::BeginDragDropTarget()) {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("FILE")) {
          auto payload_str = std::string(static_cast< char* >(payload->Data));
          LoadMaterial(payload_str);
        }
        ImGui::EndDragDropTarget();
      }
      ImGui::Columns(1);
      ImGui::PopID();
    }
    void LoadModel(std::string& payload_str) {
      auto filePath = std::filesystem::path(payload_str);
      auto ext      = filePath.extension().string();
      if (ext != ".fbx")
        return;

      auto model = Engine::AssetManager::GetModel(payload_str);
      _component->SetModel(model);
    }
    void LoadMaterial(std::string& payload_str) {
      auto filePath = std::filesystem::path(payload_str);
      auto ext      = filePath.extension().string();
      if (ext != ".mat")
        return;
      auto material = Engine::AssetManager::GetMaterial(payload_str);
      _component->SetMaterial(material);
      ;
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