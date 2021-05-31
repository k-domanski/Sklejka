#pragma once
#include "pch.h"

#include "Entity.h"
#include "Types.h"

namespace Engine::ECS {
  class Component {
    friend class EntityManager;

  private:
    EntityID _entityID;
    ComponentTypeID _typeID;
    std::string _name;

  protected:
    std::shared_ptr< Entity > _entity;

  public:
    Component(const std::string& name, ComponentTypeID id) noexcept: _name(name), _typeID(id){};
    virtual ~Component() = default;
    virtual std::string SaveToJson() {
      return "";
    }
    virtual std::string SaveToJson(std::string filePath) {
      return "";
    }
    virtual auto LoadFromJson(std::string filePath) -> void {
    }
    // virtual auto LoadFromJsonString(std::string jsonString) -> void { }
    auto GetEntityID() const -> EntityID {
      return _entityID;
    }
    auto GetEntity() const -> std::shared_ptr< Entity > {
      return _entity;
    }
    auto Name() const noexcept -> std::string {
      return _name;
    }
    auto TypeID() const noexcept -> ComponentTypeID {
      return _typeID;
    }
  };
}  // namespace Engine::ECS