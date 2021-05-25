#pragma once
#include "Types.h"

namespace Engine::ECS {
  class IComponentList {
  public:
    IComponentList()                      = default;
    IComponentList(const IComponentList&) = delete;
    auto operator=(const IComponentList&)                     = delete;
    virtual auto Remove(EntityID entityID) -> void            = 0;
    virtual auto Size() -> int                                = 0;
    virtual auto clone() -> std::shared_ptr< IComponentList > = 0;
  };

  template< typename T >
  class ComponentList : public IComponentList {
  public:
    auto GetComponent(EntityID entityID) -> std::shared_ptr< T > {
      auto it = FindIterator(entityID);
      if (it == _components.end())
        return nullptr;

      return *it;
    }
    auto AddComponent(const std::shared_ptr< T > component) -> void {
      // Only one component of type T on the Entity
      auto it = FindIterator(component->GetEntityID());
      if (it != _components.end()) {
        LOG_WARN("Component already inserted: {}", component->Name());
        return;
      }
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
    auto clone() -> std::shared_ptr< IComponentList > override {
      auto copy = std::make_shared< ComponentList< T > >();
      for (auto component : _components) {
        copy->AddComponent(std::make_shared< T >(*component));
      }

      return copy;
    }

  private:
    std::vector< std::shared_ptr< T > > _components;
    auto FindIterator(EntityID entityID) {
      return std::find_if(_components.begin(), _components.end(),
                          [entityID](auto comp) { return comp->GetEntityID() == entityID; });
    }
  };
}  // namespace Engine::ECS