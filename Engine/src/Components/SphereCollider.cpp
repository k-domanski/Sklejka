#include "pch.h"
#include "SphereCollider.h"

auto Engine::Components::SphereCollider::GetRadius() -> float {
  return _radius;
}

auto Engine::Components::SphereCollider::SetRadius(float radius) -> void {
  _radius = radius;
}

auto Engine::Components::SphereCollider::IsTrigger() -> bool {
  return _trigger;
}

auto Engine::Components::SphereCollider::SetTrigger(bool trigger) -> void {
  _trigger = trigger;
}
