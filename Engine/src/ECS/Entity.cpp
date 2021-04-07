#include "pch.h"
#include "Entity.h"

auto ECS::Entity::GetID() const -> EntityID {
  return _entityID;
}

auto ECS::Entity::GetSignature() const -> std::shared_ptr< EntitySignature > {
  return _signature;
}
