#pragma once
#include "Component.h"
#include "Types.h"
//#include "pch.h"

namespace ECS {
  class System {
  public:
    System(SystemTypeID id): _typeID(id) {
      _componentsSignatures = std::make_shared< ComponentsSignatures >();
    }
    virtual ~System() = default;

    virtual void Awake() {
    }
    virtual void Update() {
    }
    virtual void Draw() {
    }
    virtual void Destroy() {
    }
    virtual SystemTypeID GetType();

    virtual std::shared_ptr< ComponentsSignatures > GetSignatures();
    template< typename T >
    void AddSignature() const;
    template< typename T >
    void RemoveSignature() const;

    template< typename T >
    std::shared_ptr< T > AddComponent();
    void AddComponent(std::shared_ptr< Component > component);
    void RemoveComponent(std::shared_ptr< Component > component);

  private:
    SystemTypeID _typeID;
    std::shared_ptr< ComponentsSignatures > _componentsSignatures;
    std::vector< std::shared_ptr< Component > > _components;
  };

  template< typename T >
  void System::AddSignature() const {
    _componentsSignatures->insert(GetComponentTypeID< T >());
  }

  template< typename T >
  void System::RemoveSignature() const {
    _componentsSignatures->erase(GetComponentTypeID< T >());
  }

  template< typename T >
  std::shared_ptr< T > System::AddComponent() {
    const ComponentTypeID type = GetComponentTypeID< T >();

    if (_componentsSignatures->size() == 0) {
      return nullptr;
    }

    if (!(_componentsSignatures->find(type) != _componentsSignatures->end())) {
      // TODO: some exception or sth maybe
      return nullptr;
    }

    auto newComponent = std::make_shared< T >();

    _components.push_back(newComponent);

    return newComponent;
  }

}  // namespace ECS
