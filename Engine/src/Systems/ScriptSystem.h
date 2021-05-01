#pragma once
#include <ECS/System.h>

namespace Engine {
  class ScriptSystem : public ECS::System {
  public:
    ScriptSystem();
    auto Update(float deltaTime) -> void;
  };
}  // namespace Engine
