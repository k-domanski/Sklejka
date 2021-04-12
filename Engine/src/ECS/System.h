#pragma once
#include "Component.h"
//#include "ComponentList.h"
#include "Types.h"
//#include "pch.h"

namespace Engine::ECS {
  class System {
  public:
    System() = default;

    auto ComponentsCount() -> int;

    template< class T >
    auto AddSignature() -> void {
      if (ContainsSignature< T >())
        return;
      _signatures.insert(GetComponentTypeID< T >());
    }

    template< class T >
    auto RemoveSignature() -> void {
      if (!ContainsSignature< T >())
        return;
      _signatures.erase(GetComponentTypeID< T >());
    }

    template< class T >
    auto ContainsSignature() -> bool {
      auto componentSignature = GetComponentTypeID< T >();
      return _signatures.find(componentSignature) != _signatures.end();
    }

    auto ContainsSignature(ComponentTypeID componentID) -> bool;
    auto SignatureMatch(const EntitySignature& entitySignature) -> bool;

    virtual auto AddEntity(EntityID id) -> void;

    virtual void Update() = 0;

    virtual ~System() = default;

  private:
    friend class EntityManager;
    SystemSignature _signatures;

  protected:
    std::set< EntityID > _entities;
  };

}  // namespace Engine::ECS