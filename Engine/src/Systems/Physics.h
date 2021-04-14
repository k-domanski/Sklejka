#pragma once
#include "Components/SphereCollider.h"
//#include "Components/Transform.h"
#include "ECS/System.h"

namespace Engine::Systems {
  class Physics : public Engine::ECS::System {
  public:
    Physics(): System() {
      AddSignature< Engine::Components::SphereCollider >();
    }
    ~Physics() override = default;
    void Update() override;

  private:
  };
  std::map< std::pair< ECS::EntityID, std::shared_ptr< class Transform > >,
            std::vector< std::pair< ECS::EntityID, std::shared_ptr< class Transform > > > >
      _collisions;

}  // namespace Engine::Systems
