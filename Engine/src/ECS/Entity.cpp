#include "pch.h"
#include "Entity.h"

auto Engine::ECS::Entity::GetID() const -> EntityID {
  return _entityID;
}

auto Engine::ECS::Entity::GetSignature() const -> const EntitySignature& {
  return _signature;
}
