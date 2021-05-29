#include "pch.h"
#include "System.h"
#include "Component.h"

namespace Engine::ECS {
  auto System::ComponentsCount() -> int {
    return _entities.size();
  }

  auto System::ContainsSignature(ComponentTypeID componentID) -> bool {
    return _signatures.find(componentID) != _signatures.end();
  }

  auto System::SignatureMatch(const EntitySignature& entitySignature) -> bool {
    for (auto& compTypeID : _signatures) {
      auto it = entitySignature.find(compTypeID);
      if (it == entitySignature.end())
        return false;
    }
    return true;
  }

  auto System::Entities() const -> std::vector< std::shared_ptr< Entity > > {
    return _entities;
  }

  auto System::AddEntity(const std::shared_ptr< Entity >& entity) -> void {
    auto it = std::find_if(_entities.begin(), _entities.end(),
                           [&entity](auto& item) { return item->GetID() == entity->GetID(); });
    if (it != _entities.end()) {
      return;
    }
    _entities.push_back(entity);
  }

  auto System::RemoveEntity(const std::shared_ptr< Entity >& entity) -> void {
    auto it = std::find_if(_entities.begin(), _entities.end(),
                           [&entity](auto& item) { return item->GetID() == entity->GetID(); });
    if (it != _entities.end()) {
      _entities.erase(it);
    }
  }
}  // namespace Engine::ECS