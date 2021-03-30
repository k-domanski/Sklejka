#include "pch.h"
#include "EntityManager.h"

namespace ECS {
  std::shared_ptr< Entity > EntityManager::CreateEntity() {
    std::shared_ptr< Entity > newEntity = std::make_shared< Entity >();

    // TODO: add transform component?

    _entities.insert(newEntity);
    return newEntity;
  }
}  // namespace ECS
