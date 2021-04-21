#pragma once
#include <Engine.h>

namespace Editor {
  class IComponentView {
  public:
    IComponentView()          = default;
    virtual ~IComponentView() = default;
    virtual auto AttachEntity(const std::shared_ptr< Engine::ECS::Entity >& entity) -> void = 0;
    virtual auto OnDraw() -> void                                                           = 0;
    virtual auto Component() const -> std::shared_ptr< Engine::ECS::Component >             = 0;
  };
}  // namespace Editor