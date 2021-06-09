#pragma once
#include <pch.h>
#include <ECS/Component.h>

namespace Engine {
  enum Key : unsigned short;
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
    auto Detach(const std::shared_ptr< IScript >& script) -> void;
    auto Update(float deltaTime) -> void;
    auto OnKeyPressed(Key key) -> void;
    auto OnCollisionEnter(const std::shared_ptr< Components::Collider >& collider) -> void;

    template< typename T, typename... Args >
    auto Attach(Args&&... args) -> std::shared_ptr< T > {
      auto script = std::make_shared< T >(std::forward< Args >(args)...);
      script->Entity(Entity());
      _scripts.insert(script);
      script->OnCreate();

      return script;
    }

    template< typename T >
    auto GetScript() -> std::shared_ptr< T > {
      auto it = std::find_if(
          _scripts.begin(), _scripts.end(),
          [id = GetScriptTypeID< T >()](const auto& item) { return item->GetTypeID() == id; });
      if (it == _scripts.end()) {
        return nullptr;
      }
      return std::static_pointer_cast< T >(*it);
    }

  private:
    auto Entity() -> std::shared_ptr< ECS::Entity >;
  };
}  // namespace Engine
