#include "pch.h"
#include "Entity.h"
#include "ECS/EntityManager.h"
namespace Engine::ECS {
  auto Entity::GetID() const -> EntityID {
    return _entityID;
  }

  auto Entity::GetSignature() const -> std::shared_ptr< EntitySignature > {
    return _signature;
  }

  auto Entity::GetAllComponents() -> std::vector< std::shared_ptr< Component > > {
    return EntityManager::GetInstance().GetAllComponents(_entityID);
  }
}  // namespace Engine::ECS