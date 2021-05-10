#pragma once
#include <ECS/System.h>

namespace Engine {
  class NodeSystem : public ECS::System {
  public:
    NodeSystem();
    void Update(float deltaTime) override;
  };
}  // namespace Engine