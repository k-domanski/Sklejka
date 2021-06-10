#pragma once
#include <ECS/System.h>
#include <Components/Node.h>

namespace Engine {
  class NodeSystem : public ECS::System {
  public:
    NodeSystem();
    void Update(float deltaTime) override;
    auto AddEntity(const std::shared_ptr< ECS::Entity >& entity) -> void override;
    auto GetNode(int index, Engine::NodeTag tag) -> std::shared_ptr< Node >;
  };
}  // namespace Engine