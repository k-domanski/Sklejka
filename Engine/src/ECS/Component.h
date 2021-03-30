#pragma once
#include "pch.h"

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

  private:
    ComponentTypeID _typeID;
    //std::shared_ptr< Entity > _entity;
  };
}  // namespace ECS
