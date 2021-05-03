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

namespace Engine {
  class NativeScript : public ECS::Component {
  private:
    std::shared_ptr< ECS::Entity > _entity{nullptr};
    std::multiset< std::shared_ptr< IScript >, ScriptOrderOperator > _scripts;

  public:
    NativeScript();
    auto Attach(const std::shared_ptr< IScript >& script) -> void;
    auto Detach(const std::shared_ptr< IScript >& script) -> void;
    auto Update(float deltaTime) -> void;

  private:
    auto Entity() -> std::shared_ptr< ECS::Entity >;
  };
}  // namespace Engine
