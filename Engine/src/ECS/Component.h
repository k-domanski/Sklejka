#pragma once
#include "pch.h"

#include "Entity.h"
#include "Types.h"

namespace Engine::ECS {
  class Component {
    friend class EntityManager;

  private:
    EntityID _entityID;
    std::string _name;

  public:
    Component(const std::string& name) noexcept: _name(name){};
    virtual ~Component() = default;
    virtual std::string SaveToJson() {
      return "";
    }
    virtual std::string SaveToJson(std::string filePath) {
      return "";
    }
    virtual auto LoadFromJson(std::string filePath) -> void { }
    auto GetEntityID() const -> EntityID {
      return _entityID;
    }
    auto Name() const noexcept -> std::string {
      return _name;
    }
  };
}  // namespace Engine::ECS