#pragma once
#include <memory>
#include <App/KeyCodes/KeyCode.h>

namespace Engine::ECS {
  class Entity;
}
namespace Engine::Components {
  class Collider;
}

namespace Engine {
  typedef int ScriptTypeID;
  auto GetNextScriptTypeID() -> int;

  template< typename T >
  auto GetScriptTypeID() {
    const static int id = GetNextScriptTypeID();
    return id;
  }

  class IScript {
    friend class NativeScript;

  public:
    ~IScript()                                   = default;
    auto virtual OnCreate() -> void              = 0;
    auto virtual Update(float deltaTime) -> void = 0;
    auto virtual OnKeyPressed(Key key) -> void   = 0;
    auto virtual OnDestroy() -> void             = 0;
    auto virtual OnCollisionEnter(const std::shared_ptr< Components::Collider >& collider)
        -> void                                                   = 0;
    auto virtual Priority() -> int                                = 0;
    auto virtual Entity() const -> std::shared_ptr< ECS::Entity > = 0;
    auto virtual GetTypeID() const -> ScriptTypeID                = 0;

  private:
    auto virtual Entity(const std::shared_ptr< ECS::Entity >& entity) -> void = 0;
  };
}  // namespace Engine