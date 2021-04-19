#pragma once
//#include "pch.h"
#include "Types.h"

namespace Engine::ECS {
  class Entity {
    friend class EntityManager;

  public:
    Entity()  = default;
    ~Entity() = default;

    [[nodiscard]] auto GetID() const -> EntityID;
    [[nodiscard]] auto GetSignature() const -> std::shared_ptr< EntitySignature >;

    template< class T, typename... Args >
    auto AddComponent(Args&&... args) -> std::shared_ptr< T > {
      return EntityManager::GetInstance().AddComponent< T >(_entityID,
                                                            std::forward< Args >(args)...);
    }

    template< class T >
    std::shared_ptr< T > GetComponent() {
      return EntityManager::GetInstance().GetComponent< T >(_entityID);
    }

    auto Name() const noexcept -> std::string {
      return _name;
    }
    auto Name(const std::string& name) -> std::string {
      return _name = name;
    }

  private:
    EntityID _entityID{0};
    std::string _name{"Entity"};
    std::shared_ptr< EntitySignature > _signature{std::make_shared< EntitySignature >()};
  };
}  // namespace Engine::ECS
