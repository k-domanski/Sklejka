#pragma once

#include "ECS.h"
//#include "Entity.h"
#include "ComponentList.h"
//#include "System.h"
#include "Engine/SceneManager.h"
#include "Engine/Scene.h"

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
      if (SceneManager::GetCurrentScene()->_componentLists.count(compTypeID) == 0) {
        AddComponentList< T >();
      }
      return std::static_pointer_cast< ComponentList< T > >(
          SceneManager::GetCurrentScene()->_componentLists[compTypeID]);
      // return static_cast< ComponentList< T > >(*_componentLists[compTypeID]);
    }

    template< typename T, typename... Args >
    auto AddComponent(EntityID entityID, Args&&... args) -> std::shared_ptr< T > {
      auto compTypeID = GetComponentTypeID< T >();
      auto entity     = GetEntity(entityID);
      auto it         = entity->_signature->find(compTypeID);
      assert(it == entity->_signature->end());
      entity->_signature->insert(compTypeID);

      // TODO: Create Component Instance
      auto component       = std::make_shared< T >(std::forward< Args >(args)...);
      component->_entityID = entityID;
      auto list            = GetComponentList< T >();
      list->AddComponent(component);

      UpdateEntity(entity);
      return component;
    }

    template< class T >
    auto GetComponentImpl(EntityID entityID) -> std::shared_ptr< T > {
      auto compTypeID = GetComponentTypeID< T >();
      auto list       = GetComponentList< T >();
      /*for (auto element : *list) {
        if (element.GetEntityID() == entityID)
          return element;
      }*/
      return list->GetComponent(entityID);

      // return nullptr;
    }
    template< class T >
    static auto GetComponent(EntityID entityID) -> std::shared_ptr< T > {
      return GetInstance().GetComponentImpl< T >(entityID);
    }

    template< class T >
    auto RemoveComponent(EntityID entityID) -> void {
      auto compTypeID = GetComponentTypeID< T >();

      auto entity = GetEntity(entityID);
      auto it     = entity->_signature->find(compTypeID);
      if (it == entity->_signature->end())
        return;
      entity->_signature->erase(it);

      auto list = GetComponentList< T >();
      list->Remove(entityID);
    }

    template< class T >
    auto RegisterSystem() -> std::shared_ptr< T > {
      auto systemID = GetSystemTypeID< T >();
      if (Engine::SceneManager::GetCurrentScene()->_registeredSystems.count(systemID) == 0) {
        /*auto system                  = std::make_shared< T >();
        _registeredSystems[systemID] = std::move(system);*/
        SceneManager::GetCurrentScene()->_registeredSystems[systemID] = std::make_shared< T >();
        UpdateSystem(systemID);
      }
      return std::static_pointer_cast< T >(
          SceneManager::GetCurrentScene()->_registeredSystems[systemID]);
    }
    template< typename T >
    auto GetSystem() -> std::shared_ptr< T > {
      auto scene = Engine::SceneManager::GetCurrentScene();
      if (scene == nullptr) {
        return nullptr;
      }
      auto id = GetSystemTypeID< T >();
      if (scene->_registeredSystems.count(id) == 0) {
        return nullptr;
      }
      return std::static_pointer_cast< T >(scene->_registeredSystems[id]);
    }

    auto UpdateSystem(SystemTypeID systemID) -> void;
    auto UpdateEntity(std::shared_ptr< Entity > entity) -> void;

    auto BelongsToSystem(SystemTypeID systemID, EntityID entityID) -> bool;

    auto AddToSystem(SystemTypeID systemID, EntityID entityID) -> void;
    auto RemoveFromSystem(SystemTypeID systemID, EntityID entityID) -> void;

    auto CreateEntity() -> std::shared_ptr< Entity >;
    auto CreateEntity(EntityID id) -> std::shared_ptr< Entity >;
    auto GetEntity(EntityID) -> std::shared_ptr< Entity >;
    auto RemoveEntity(EntityID id) -> void;
    auto GetAllComponents(EntityID id) -> std::vector< std::shared_ptr< Component > >;
    auto Update(float deltaTime) -> void;
    auto Clear() -> void;
    static auto InjectEntity(const std::shared_ptr< Entity >& entity) -> void;
    template< typename T >
    static auto InjectComponent(const std::shared_ptr< T >& component) -> void {
      auto list      = GetInstance().GetComponentList< T >();
      auto& instance = GetInstance();
      list->AddComponent(component);
      instance.UpdateEntity(instance.GetEntity(component->GetEntityID()));
    }

    // auto Draw() -> void;

  private:
    EntityManager() = default;
    /*std::vector< std::shared_ptr< Entity > > _entities;
    std::map< ComponentTypeID, std::shared_ptr< IComponentList > > _componentLists;
    std::map< SystemTypeID, std::shared_ptr< System > > _registeredSystems;*/

    template< typename T >
    auto AddComponentList() -> void {
      auto compTypeID = GetComponentTypeID< T >();
      SceneManager::GetCurrentScene()->_componentLists[compTypeID] =
          std::make_shared< ComponentList< T > >();
    }
  };
}  // namespace Engine::ECS
