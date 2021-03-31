#include "pch.h"
#include "System.h"
#include "Component.h"

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
}  // namespace ECS