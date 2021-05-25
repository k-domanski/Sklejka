#pragma once
//#include "pch.h"
#include "Types.h"

namespace Engine::ECS {
  class Entity {
    friend class EntityManager;

  public:
    Entity() = default;
    Entity(const Entity& entity);
    /*TODO: Entity(const std::string& name);
      lub overload CreateEntity w EntityManager*/
    ~Entity() = default;

    [[nodiscard]] auto GetID() const -> EntityID;
    [[nodiscard]] auto GetSignature() const -> std::shared_ptr< EntitySignature >;

    template< class T, typename... Args >
    auto AddComponent(Args&&... args) -> std::shared_ptr< T > {
      return EntityManager::GetInstance().AddComponent< T >(_entityID,
                                                            std::forward< Args >(args)...);
    }

    template< class T >
    auto RemoveComponent() {
      return EntityManager::GetInstance().RemoveComponent< T >(_entityID);
    }

    template< class T >
    std::shared_ptr< T > GetComponent() {
      return EntityManager::GetInstance().GetComponent< T >(_entityID);
    }

    auto GetAllComponents() -> std::vector< std::shared_ptr< class Component > >;

    template< typename T >
    auto HasComponent() -> bool {
      auto it = _signature->find(GetComponentTypeID< T >());
      return it != _signature->end();
    }

    auto Name() const noexcept -> std::string {
      return _name;
    }
    auto Name(const std::string& name) -> std::string {
      return _name = name;
    }

    std::string SaveToJson(size_t parentID);
    std::string SaveToJson(std::string filepath, size_t parentID);
    void LoadFromJson(std::string filepath);
    EntityID GetParentFromJson(std::string json_string);

  private:
    EntityID _entityID{0};
    std::string _name{"Entity"};
    std::shared_ptr< EntitySignature > _signature{std::make_shared< EntitySignature >()};
  };
}  // namespace Engine::ECS
