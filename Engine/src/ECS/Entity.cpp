#include "pch.h"
#include "Entity.h"
namespace Engine::ECS {
  auto Entity::GetID() const -> EntityID {
    return _entityID;
  }

  auto Entity::GetSignature() const -> std::shared_ptr< EntitySignature > {
    return _signature;
  }
}  // namespace Engine::ECS