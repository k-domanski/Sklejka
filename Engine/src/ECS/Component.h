#pragma once
#include "pch.h"


#include "Entity.h"
#include "Types.h"

namespace ECS {

  class Component {
  public:
    Component(const ComponentTypeID id): _typeID(id) {
    }

    virtual ~Component() = default;
    virtual ComponentTypeID GetTypeID() {
      return _typeID;
    }
    virtual void AddToEntity(std::shared_ptr< Entity > entity)
    {
      entity->insert(_typeID);
      _entity = entity;
    }
    virtual void RemoveFromEntity()
    {
      _entity->erase( _typeID);
      _entity = nullptr;
    }

  private:
    ComponentTypeID _typeID;
    std::shared_ptr< Entity > _entity;
  };
}  // namespace ECS
