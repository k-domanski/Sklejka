#pragma once
#include "ECS/Component.h"

namespace Engine::Components {
  class SphereCollider : public Engine::ECS::Component {
  public:
    SphereCollider(): Component(), _radius(1.0f), _triggger(false) {
    }
    ~SphereCollider() override = default;

    auto GetRadius() -> float;
    auto SetRadius(float radius) -> void;
    auto IsTrigger() -> bool;
    auto SetTrigger(bool trigger) -> void;

  private:
    float _radius;
    bool _triggger;
  };

}  // namespace Engine::Components
