#pragma once
#include "Types.h"

namespace ECS {
  class IComponentList {
  public:
    IComponentList()                      = default;
    IComponentList(const IComponentList&) = delete;
    auto operator=(const IComponentList&)          = delete;
    virtual auto Remove(EntityID entityID) -> void = 0;
    virtual auto Size() -> int                     = 0;
  };

  template< typename T >
  class ComponentList : public IComponentList {
  public:
    auto GetComponent(EntityID entityID) -> T& {
      auto it = FindIterator(entityID);
      assert(it != _components.end());
      return *it;
    }
    auto AddComponent(const T& component) -> void {
      // Only one component of type T on the Entity
      auto it = FindIterator(component.GetEntityID());
      if (it == _components.end())
        _components.push_back(component);
    }
    auto Remove(EntityID entityID) -> void override {
      auto it = FindIterator(entityID);
      if (it != _components.end())
        _components.erase(it);
    }

    auto Size() -> int override {
      return _components.size();
    }

  private:
    std::vector< T > _components;
    auto FindIterator(EntityID entityID) {
      return std::find_if(_components.begin(), _components.end(),
                          [entityID](auto comp) { return comp.GetEntityID() == entityID; });
    }
  };
}  // namespace ECS