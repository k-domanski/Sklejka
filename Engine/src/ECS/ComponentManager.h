#pragma once
#include "Types.h"
#include "Component.h"

namespace Engine::ECS {
  template< class T >
  class ComponentManager {
  public:
    ComponentManager() {
      _components          = std::make_shared< std::set< std::shared_ptr< T > > >();
      _componentIDIterator = 0u;
    }
    virtual ~ComponentManager() = default;

    [[nodiscard]] ComponentTypeID GetStoredComponentTypeID() const {
      return GetComponentTypeID< T >();
    }

    std::shared_ptr< T > CreateComponent(std::shared_ptr<Entity> forEntity) {
      std::shared_ptr< T > newComponent = std::make_shared< T >(_componentIDIterator++, forEntity);
      _components->insert(newComponent);
      return newComponent;
    }

    void RemoveComponent(ComponentID id) {
      std::shared_ptr< T > componentToRemove = GetComponent(id);
      if (componentToRemove != nullptr)
        _components->erase(componentToRemove);
    }

    std::shared_ptr< T > GetComponent(ComponentID id) {
      for (auto component : _components) {
        if (component->GetID() == id) {
          return component;
        }
      }
      return nullptr;
    }

  private:
    std::shared_ptr< std::set< std::shared_ptr< T > > > _components;
    ComponentID _componentIDIterator;
  };
}  // namespace ECS