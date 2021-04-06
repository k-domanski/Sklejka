#pragma once
#include "Component.h"
#include "ComponentList.h"
#include "Types.h"
//#include "pch.h"

namespace ECS {
  class System {
  public:
    System() = default;

    auto ComponentsCount() -> int {
      return _entities.size();
    }

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

    auto ContainsSignature(ComponentTypeID componentID) -> bool {
      return _signatures.find(componentID) != _signatures.end();
    }

    virtual void Update() = 0;
    virtual void Draw()   = 0;

    virtual ~System() = default;

  private:
    friend class EntityManager;
    std::set< EntityID > _entities;
    SystemSignature _signatures;
  };

}  // namespace ECS