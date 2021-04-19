#include "pch.h"
#include "SphereCollider.h"

#include <nlohmann/json.hpp>

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

std::string Engine::Components::SphereCollider::SaveToJson(std::string filePath)
{
  nlohmann::json json = nlohmann::json{{"componentType", "sphereCollider"},
                                       {"radius", _radius},
                                       {"isTrigger", _trigger}};

  std::ofstream ofstream;
  ofstream.open(filePath);
  ofstream << json.dump(4);
  ofstream.close();

  return json.dump(4);
}
