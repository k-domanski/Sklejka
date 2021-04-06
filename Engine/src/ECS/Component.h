#pragma once
#include "pch.h"

#include "Entity.h"
#include "Types.h"

namespace ECS {
  class Component {
    friend class EntityManager;

  private:
    EntityID _entityID;

  public:
    auto GetEntityID() const -> EntityID {
      return _entityID;
    }
  };
}  // namespace ECS