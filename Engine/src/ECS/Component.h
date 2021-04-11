#pragma once
#include "pch.h"

#include "Entity.h"
#include "Types.h"

namespace Engine::ECS {
  class Component {
    friend class EntityManager;

  private:
    EntityID _entityID;

  public:
    Component()          = default;
    virtual ~Component() = default;
    auto GetEntityID() const -> EntityID {
      return _entityID;
    }
  };
}  // namespace Engine::ECS