#pragma once
#include <ECS/System.h>

namespace Engine {
  enum Key : unsigned short;
}

namespace Engine::Systems {
  class ScriptSystem : public ECS::System {
  public:
    ScriptSystem();
    auto Update(float deltaTime) -> void;
    auto OnKeyPressed(Key key) -> void;
  };
}  // namespace Engine::Systems
