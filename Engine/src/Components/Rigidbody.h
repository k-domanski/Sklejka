#pragma once
#include "ECS/Component.h"

namespace Engine::Components
{
  class Rigidbody : public Engine::ECS::Component {
  public:
    Rigidbody(): Component() {
      _velocity   = glm::vec3(0.0f);
      _useGravity = true;
    }
    ~Rigidbody() override = default;

    auto GetVelocity() -> glm::vec3;
    auto SetVelocity(glm::vec3 velocity) -> void;
    auto UseGravity() -> bool;
    auto SetGravity(bool gravity) -> void;
    auto IsKinematic() -> bool;
    auto SetKinematic(bool kinematic) -> void;

  private:
    glm::vec3 _velocity;
    bool _useGravity;
    bool _kinematic;
  };
}
