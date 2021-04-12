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

  auto System::AddEntity(EntityID id) -> void
  {
    if (_entities.find(id) != _entities.end())
      return;
    _entities.insert(id);
  }
}  // namespace ECS