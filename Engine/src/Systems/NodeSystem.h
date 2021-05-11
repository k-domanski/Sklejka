#pragma once
#include <ECS/System.h>
#include <Components/Node.h>

namespace Engine {
  class NodeSystem : public ECS::System {
  public:
    NodeSystem();
    void Update(float deltaTime) override;
    auto AddEntity(ECS::EntityID id) -> void override;
    auto GetNode(int index) -> std::shared_ptr< Node >;
  };
}  // namespace Engine