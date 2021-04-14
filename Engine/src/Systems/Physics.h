#pragma once
#include "Components/SphereCollider.h"
//#include "Components/Transform.h"
#include "Components/BoxCollider.h"
#include "Components/Rigidbody.h"
#include "Components/Transform.h"
#include "ECS/System.h"

namespace Engine::Systems {
  class Physics : public Engine::ECS::System {
  public:
    Physics(): System() {
      AddSignature< Components::BoxCollider >();
      AddSignature< Components::Rigidbody >();
      AddSignature< Transform >();
      _gravity = glm::vec3(0.0f, 10.0f, 0.0f);
    }
    ~Physics() override = default;
    void Update(float deltaTime) override;

  private:
    std::map< ECS::EntityID, std::vector< ECS::EntityID > > _collisions;
    glm::vec3 _gravity;
  };

}  // namespace Engine::Systems
