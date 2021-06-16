#pragma once
#include <pch.h>
#include "Types.h"
#include <Utility/Helpers.h>
#include <Utility/Utility.h>

namespace Engine {
  BETTER_ENUM(LayerBit, uint32_t, Default = Helpers::Bit32(0), Player = Helpers::Bit32(1),
              Acorn = Helpers::Bit32(2), Bell = Helpers::Bit32(3), Boss = Helpers::Bit32(4));
  typedef Utility::BitFlags< LayerBit > LayerMask;
}  // namespace Engine

namespace Engine::ECS {
  class Entity {
    friend class EntityManager;

  public:
    Entity() {
      layer.Set(LayerMask::Flag::Default);
      collisionLayer.SetAll();
    }
    /*TODO: Entity(const std::string& name);
      lub overload CreateEntity w EntityManager*/
    ~Entity() = default;
    [[nodiscard]] auto GetID() const -> EntityID;
    [[nodiscard]] auto GetSignature() const -> std::shared_ptr< EntitySignature >;

    template< class T, typename... Args >
    auto AddComponent(Args&&... args) -> std::shared_ptr< T > {
      return EntityManager::GetInstance().AddComponent< T >(_entity.lock(),
                                                            std::forward< Args >(args)...);
    }

    template< class T >
    auto RemoveComponent() {
      EntityManager::GetInstance().RemoveComponent< T >(_entity.lock());
    }

    auto RemoveComponent(ComponentTypeID componentType) -> void;

    template< class T >
    std::shared_ptr< T > GetComponent() {
      return EntityManager::GetInstance().GetComponent< T >(_entity.lock());
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
    void LoadFromJson(std::string filepath, bool overrideID = false);
    EntityID GetParentFromJson(std::string json_string);

  public:
    LayerMask layer;
    LayerMask collisionLayer;

  private:
    EntityID _entityID{0};
    std::weak_ptr< Entity > _entity;
    std::string _name{"Entity"};
    std::shared_ptr< EntitySignature > _signature{std::make_shared< EntitySignature >()};
    std::unordered_map< ComponentTypeID, std::shared_ptr< Component > > _componentCache;
  };
}  // namespace Engine::ECS
