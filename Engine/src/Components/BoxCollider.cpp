#include "pch.h"
#include "BoxCollider.h"

auto Engine::Components::BoxCollider::GetSize() -> glm::vec3 {
  return _size;
}

auto Engine::Components::BoxCollider::SetSize(glm::vec3 size) -> void {
  _size = size;
}

auto Engine::Components::BoxCollider::IsTrigger() -> bool {
  return _trigger;
}

auto Engine::Components::BoxCollider::SetTrigger(bool trigger) -> void {
  _trigger = trigger;
}
