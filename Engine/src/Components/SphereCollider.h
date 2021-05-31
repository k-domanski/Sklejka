#pragma once
#include "ECS/Component.h"
#include <ECS/Types.h>

namespace Engine::Components {
  class SphereCollider : public ECS::Component {
  public:
    SphereCollider(): Component("Sphere Collider", ECS::GetComponentTypeID<SphereCollider>()), _radius(1.0f), _trigger(false) {
    }
    ~SphereCollider() override = default;

    auto GetRadius() -> float;
    auto SetRadius(float radius) -> void;
    auto IsTrigger() -> bool;
    auto SetTrigger(bool trigger) -> void;
    std::string SaveToJson(std::string filePath) override;
  private:
    float _radius;
    bool _trigger;
  };

}  // namespace Engine::Components
