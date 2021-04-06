#include "pch.h"
#include "Entity.h"

auto ECS::Entity::GetID() const -> EntityID {
  return _entityID;
}

auto ECS::Entity::GetSignature() const -> const EntitySignature& {
  return _signature;
}
