#pragma once
#include <memory>

namespace Engine::ECS {
  class Entity;
}

namespace Engine {
  class IScript {
    friend class NativeScript;

  public:
    ~IScript()                                                    = default;
    auto virtual OnCreate() -> void                               = 0;
    auto virtual Update(float deltaTime) -> void                  = 0;
    auto virtual OnDestroy() -> void                              = 0;
    auto virtual Priority() -> int                                = 0;
    auto virtual Entity() const -> std::shared_ptr< ECS::Entity > = 0;

  private:
    auto virtual Entity(const std::shared_ptr< ECS::Entity >& entity) -> void = 0;
  };
}  // namespace Engine