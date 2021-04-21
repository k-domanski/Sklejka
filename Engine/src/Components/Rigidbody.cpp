#include "pch.h"
#include "Rigidbody.h"

#include "nlohmann/json.hpp"

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

std::string Engine::Components::Rigidbody::SaveToJson()
{
  using namespace nlohmann;
  json json = nlohmann::json{
      {"componentType", "rigidbody"},
      {"velocity", {{"x", _velocity.x}, {"y", _velocity.y}, {"z", _velocity.z}}},
      {"useGravity", _useGravity},
      {"kinematic", _kinematic},
  };

  return json.dump(4);
}

std::string Engine::Components::Rigidbody::SaveToJson(std::string filePath)
{
  std::ofstream ofstream;
  ofstream.open(filePath);
  ofstream << SaveToJson();
  ofstream.close();

  return SaveToJson();
}


