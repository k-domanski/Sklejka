#pragma once
#include "IComponentView.h"
#include <Engine.h>
#include "AttributeView.h"
#include "Components/Collider.h"
#include "Components/Rigidbody.h"
#include "Components/DirectionalLight.h"
#include "Components/Node.h"
#include "Components/Animator.h"
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

      if (hasModel) {
        int mesh_index = _component->MeshIndex();
        if (ImGui::InputInt("Mesh Index", &mesh_index)) {
          mesh_index = glm::clamp(mesh_index, 0, _component->GetModel()->GetMeshCount() - 1);
          _component->MeshIndex(mesh_index);
        }
      }

      ImGui::PushID("Model");
      ImGui::Columns(2);
      ImGui::SetColumnWidth(0, 100);
      ImGui::Text("Model");
      ImGui::NextColumn();
      if (ImGui::BeginCombo("", modelPath.c_str())) {
        for (auto& entry : std::filesystem::recursive_directory_iterator(_modelsFolder)) {
          const auto path     = entry.path();
          const auto filename = path.filename().string();
          auto ext            = path.extension().string();
          std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
          if (!(ext == ".fbx" || ext == ".obj")) {
            continue;
          }
          if (ImGui::Selectable(filename.c_str())) {
            auto model = Engine::AssetManager::GetModel(path.string());
            _component->SetModel(model);
          }
        }
        ImGui::EndCombo();
      }
      if (ImGui::BeginPopupContextItem("component context menu")) {
        if (ImGui::Selectable("Clear")) {
          _component->SetModel(nullptr);
        }
        ImGui::EndPopup();
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
      if (ImGui::BeginPopupContextItem("component context menu")) {
        if (ImGui::Selectable("Clear")) {
          _component->SetMaterial(nullptr);
        }
        ImGui::EndPopup();
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
      std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
      if (!(ext == ".fbx" || ext == ".obj")) {
        return;
      }

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
  /*Directional Light*/
  class DirectionalLightView : public ComponentView< Engine::DirectionalLight > {
    auto OnDraw() -> void override {
      float intensity = _component->Intensity();
      if (DrawFloat("Intensity", intensity, 0.0f, 1.0f)) {
        _component->Intensity(intensity);
      }

      float shadow_bias = _component->ShadowBias();
      if (DrawFloat("Shadow Bias", shadow_bias, 0.0f, 1.0f)) {
        _component->ShadowBias(shadow_bias);
      }

      glm::vec3 ambient = _component->Ambient();
      if (ImGui::CollapsingHeader("Ambient##h")) {
        ImGui::PushItemWidth(125);
        ImGui::ColorPicker3("Ambient##p", &ambient[0]);
        ImGui::PopItemWidth();
      }
      _component->Ambient(ambient);

      glm::vec3 diffuse = _component->Diffuse();
      if (ImGui::CollapsingHeader("Diffuse##h")) {
        ImGui::PushItemWidth(125);
        ImGui::ColorPicker3("Diffuse##p", &diffuse[0]);
        ImGui::PopItemWidth();
      }
      _component->Diffuse(diffuse);
    }
  };

  /* Node */
  class NodeView : public ComponentView< Engine::Node > {
    auto OnDraw() -> void override {
      auto index = _component->Index();
      ImGui::LabelText("Self Index##A", "Self Index");
      if (ImGui::InputInt("Index_Self##1", &index)) {
        _component->Index(index);
      }
      ImGui::LabelText("Next Index##B", "Next Index");
      auto next_index = _component->NextIndex();
      if (ImGui::InputInt("Index_Next##2", &next_index)) {
        _component->NextIndex(next_index);
      }
    }
  };

  class AnimatorView : public ComponentView<Engine::Animator> {
  private:
      std::string modelPath;
      std::filesystem::path _modelsFolder = std::filesystem::current_path().string() + "\\models\\";
  public:
      auto OnDraw() -> void override {
          bool hasAnimation = _component->GetAnimation() != nullptr;
          
          modelPath =
              hasAnimation
              ? std::filesystem::path(_component->GetAnimation()->GetFilePath()).filename().string()
              : "<None>";
          ImGui::PushID("Animation");

          ImGui::Columns(2);
          ImGui::SetColumnWidth(0, 100);
          ImGui::Text("Animation");
          ImGui::NextColumn();
          if (ImGui::BeginCombo("", modelPath.c_str())) {
              for (auto& entry : std::filesystem::recursive_directory_iterator(_modelsFolder)) {
                  const auto path = entry.path();
                  const auto filename = path.filename().string();
                  auto ext = path.extension().string();
                  std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
                  if (!(ext == ".fbx" || ext == ".obj")) {
                      continue;
                  }
                  if (ImGui::Selectable(filename.c_str())) {
                      auto model = Engine::AssetManager::GetModel(path.string());
                      _component->SetAnimation(model);
                  }
              }
              ImGui::EndCombo();
          }
          if (ImGui::BeginPopupContextItem("component context menu")) {
              if (ImGui::Selectable("Clear")) {
                  _component->SetAnimation(nullptr);
              }
              ImGui::EndPopup();
          }
          if (ImGui::BeginDragDropTarget()) {
              if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("FILE")) {
                  auto payload_str = std::string(static_cast<char*>(payload->Data));
                  LoadModel(payload_str);
              }
              ImGui::EndDragDropTarget();
          }
          ImGui::Columns(1);
          ImGui::PopID();
          ImGui::Separator();
      }

      void LoadModel(std::string& payload_str) {
          auto filePath = std::filesystem::path(payload_str);
          auto ext = filePath.extension().string();
          std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
          if (!(ext == ".fbx" || ext == ".obj")) {
              return;
          }

          auto model = Engine::AssetManager::GetModel(payload_str);
          _component->SetAnimation(model);
      }
  };
}  // namespace Editor