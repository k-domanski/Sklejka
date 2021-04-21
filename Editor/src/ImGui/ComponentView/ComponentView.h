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
  public:
    auto OnDraw() -> void override {
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

      // if (_component->UseGravity())
      // rbGravity = true;
      DrawBool("Use Gravity", rbGravity);
      _component->SetGravity(rbGravity);

      // if (_component->IsKinematic())
      // rbKinematic = true;
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

      // if (_component->IsTrigger)
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