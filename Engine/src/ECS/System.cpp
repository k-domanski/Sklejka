#include "pch.h"
#include "System.h"

namespace ECS {
  SystemTypeID System::GetType() {
    return _typeID;
  }

  std::shared_ptr< ComponentsSignatures > System::GetSignatures() {
    return _componentsSignatures;
  }

  void System::AddComponent(std::shared_ptr<Component> component)
  {
    if (!(_componentsSignatures->find(component->GetTypeID()) != _componentsSignatures->end()))
    return;

    _components.push_back(component);
  }

  void System::RemoveComponent(std::shared_ptr<Component> component)
  {
    if (!(_componentsSignatures->find(component->GetTypeID()) != _componentsSignatures->end()))
      return;

    //TODO: check if works
    _components.erase(std::remove(_components.begin(), _components.end(), component));
  }

  template< typename T >
  void System::AddSignature() const {
    _componentsSignatures->insert(GetComponentTypeID< T >());
  }

  template< typename T >
  void System::RemoveSignature() const {
    _componentsSignatures->erase(GetComponentTypeID< T >());
  }

  template< typename T >
  std::shared_ptr< Component > System::AddComponent() {
    const ComponentTypeID type = GetComponentTypeID< T >();

    if (!(_componentsSignatures->find(type) != _componentsSignatures->end())) {
      // TODO: some exception or sth maybe
      return nullptr;
    }

    auto newComponent = std::make_shared< T >();

    _components.push_back(newComponent);

    return newComponent;
  }
}  // namespace ECS