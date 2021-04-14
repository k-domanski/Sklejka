#include "pch.h"
#include "Rigidbody.h"

auto Engine::Components::Rigidbody::GetVelocity() -> glm::vec3 {
  return _velocity;
}

auto Engine::Components::Rigidbody::SetVelocity(glm::vec3 velocity) -> void {
  _velocity = velocity;
}

auto Engine::Components::Rigidbody::UseGravity() -> bool {
  return _useGravity;
}

auto Engine::Components::Rigidbody::SetGravity(bool gravity) -> void {
  _useGravity = gravity;
}

auto Engine::Components::Rigidbody::IsKinematic() -> bool
{
  return _kinematic;
}

auto Engine::Components::Rigidbody::SetKinematic(bool kinematic) -> void
{
  _kinematic = kinematic;
}
