#pragma once

#include "Entity.h"
#include "ComponentList.h"
#include "System.h"

namespace Engine::ECS {
  class EntityManager {
  public:
    ~EntityManager()                    = default;
    EntityManager(EntityManager const&) = delete;
    void operator=(EntityManager const&) = delete;

    static EntityManager& GetInstance() {
      static EntityManager instance;

      return instance;
    }

    template< typename T >
    auto GetComponentList() -> std::shared_ptr< ComponentList< T > > {
      auto compTypeID = GetComponentTypeID< T >();
      if (_componentLists.count(compTypeID) == 0) {
        AddComponentList< T >();
      }
      return std::static_pointer_cast< ComponentList< T > >(_componentLists[compTypeID]);
      // return static_cast< ComponentList< T > >(*_componentLists[compTypeID]);
    }

    template< typename T, typename... Args >
    auto AddComponent(EntityID entityID, Args&&... args) -> T& {
      auto compTypeID = GetComponentTypeID< T >();
      auto& entity    = GetEntity(entityID);
      auto it         = entity._signature.find(compTypeID);
      assert(it == entity._signature.end());
      entity._signature.insert(compTypeID);
      UpdateEntity(entity);

      // TODO: Create Component Instance
      T component(std::forward< Args >(args)...);
      component._entityID = entityID;
      auto& list          = GetComponentList< T >();
      list->AddComponent(component);
      return list->GetComponent(entityID);
    }

    template< class T >
    auto GetComponent(EntityID entityID) -> T& {
      auto compTypeID = GetComponentTypeID< T >();
      auto list       = GetComponentList< T >();
      for (auto element : *list) {
        if (element.GetEntityID() == entityID)
          return element;
      }

      return nullptr;
    }

    template< class T >
    auto RegisterSystem() -> void {
      auto systemID = GetSystemTypeID< T >();
      if (_registeredSystems.count(systemID) == 0) {
        auto system                  = std::make_shared< T >();
        _registeredSystems[systemID] = std::move(system);
        UpdateSystem(systemID);
      }
    }

    auto UpdateSystem(SystemTypeID systemID) -> void;
    auto UpdateEntity(Entity entity) -> void;

    auto BelongsToSystem(SystemTypeID systemID, EntityID entityID) -> bool;

    auto AddToSystem(SystemTypeID systemID, EntityID entityID) -> void;

    auto CreateEntity() -> Entity&;
    auto GetEntity(EntityID) -> Entity&;
    auto Update() -> void;

    auto Draw() -> void;

  private:
    EntityManager() = default;
    std::vector< Entity > _entities;
    std::map< ComponentTypeID, std::shared_ptr< IComponentList > > _componentLists;
    std::map< SystemTypeID, std::shared_ptr< System > > _registeredSystems;

    template< typename T >
    auto AddComponentList() -> void {
      auto compTypeID             = GetComponentTypeID< T >();
      _componentLists[compTypeID] = std::make_shared< ComponentList< T > >();
    }
  };
}  // namespace ECS
