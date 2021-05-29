#pragma once
#include <pch.h>
#include <ECS/Component.h>

namespace Engine {
  class IScript;
  struct ScriptOrderOperator {
    auto operator()(const std::shared_ptr< IScript >& lhs,
                    const std::shared_ptr< IScript >& rhs) const -> bool;
  };
}  // namespace Engine

namespace Engine::Components {
  class Collider;
}

namespace Engine {
  class NativeScript : public ECS::Component {
  private:
    std::multiset< std::shared_ptr< IScript >, ScriptOrderOperator > _scripts;

  public:
    NativeScript();
    auto Attach(const std::shared_ptr< IScript >& script) -> void;
    template< typename T, typename... Args >
    auto Attach(Args&&... args) -> std::shared_ptr< T > {
      auto script = std::make_shared< T >(std::forward< Args >(args)...);
      script->Entity(Entity());
      _scripts.insert(script);
      script->OnCreate();

      return script;
    }
    auto Detach(const std::shared_ptr< IScript >& script) -> void;
    auto Update(float deltaTime) -> void;
    auto OnCollisionEnter(const std::shared_ptr< Components::Collider >& collider) -> void;

  private:
    auto Entity() -> std::shared_ptr< ECS::Entity >;
  };
}  // namespace Engine
