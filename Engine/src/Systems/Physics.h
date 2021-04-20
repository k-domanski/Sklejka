#pragma once
//#include "Components/SphereCollider.h"
//#include "Components/Transform.h"
//#include "../../GJK.h"
//#include "Components/BoxCollider.h"
#include "Components/Collider.h"
#include "Components/Rigidbody.h"
#include "Components/Transform.h"
#include "Utility/GJK.h"
//#include "ECS/System.h"

namespace Engine::Systems {
  class Physics : public Engine::ECS::System {
  public:
    Physics(): System() {
      AddSignature< Components::Collider >();
      AddSignature< Components::Rigidbody >();
      AddSignature< Transform >();

      _gravity = glm::vec3(0.0f, 10.0f, 0.0f);
    }
    ~Physics() override = default;
    void Update(float deltaTime) override;

  private:
    std::map< ECS::EntityID, std::vector< ECS::EntityID > > _collisions;
    glm::vec3 _gravity;

    auto CheckCollision(std::shared_ptr< Components::Collider > c1, std::shared_ptr< Transform > t1,
                        std::shared_ptr< Components::Collider > c2, std::shared_ptr< Transform > t2)
        -> bool;
    auto CreateSphereShape(std::shared_ptr< Components::Collider > c,
                           std::shared_ptr< Transform > t) -> Utility::GJK::Shape;
    auto CreateBoxShape(std::shared_ptr< Components::Collider > c, std::shared_ptr< Transform > t)
        -> Utility::GJK::Shape;
    auto ResolveCollisions(std::shared_ptr< Components::Collider > c1,
                           std::shared_ptr< Transform > t1,
                           bool static1, std::shared_ptr< Components::Collider > c2,
                           std::shared_ptr< Transform > t2, bool static2) -> void;
  };

}  // namespace Engine::Systems
