#include "pch.h"
#include "System.h"
#include "Component.h"

namespace Engine::ECS {
  auto System::ComponentsCount() -> int
  {
    return _entities.size();
  }

  auto System::ContainsSignature(ComponentTypeID componentID) -> bool
  {
    return _signatures.find(componentID) != _signatures.end();
  }

  auto System::SignatureMatch(const EntitySignature& entitySignature) -> bool {
    for(auto& compTypeID : _signatures) {
      auto it = entitySignature.find(compTypeID);
      if (it == entitySignature.end())
        return false;
    }
    return true;
  }

  auto System::AddEntity(EntityID id) -> void
  {
    if (_entities.find(id) != _entities.end())
      return;

    _entities.insert(id);
  }

  auto System::RemoveEntity(EntityID id) -> void
  {
    if (_entities.find(id) != _entities.end())
      _entities.erase(id);
  }
}  // namespace ECS