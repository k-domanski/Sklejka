#pragma once
//#include "pch.h"
#include "Types.h"

namespace ECS {
  class Entity {
    friend class EntityManager;

  public:
    Entity(): _entityID(0), _signature(std::make_shared< EntitySignature >()) {
    }

    ~Entity() = default;

    [[nodiscard]] auto GetID() const -> EntityID;
    [[nodiscard]] auto GetSignature() const -> std::shared_ptr< EntitySignature >;

    template< class T, typename... Args >
    void AddComponent(Args&&... args) {
      EntityManager::GetInstance().AddComponent< T >(_entityID, std::forward< Args >(args)...);
    }

    template< class T >
    T& GetComponent() {
      return EntityManager::GetInstance().GetComponent< T >(_entityID);
    }

  private:
    EntityID _entityID;
    std::shared_ptr< EntitySignature > _signature;
  };
}  // namespace ECS
