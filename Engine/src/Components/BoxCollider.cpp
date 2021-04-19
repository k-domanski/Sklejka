#include "pch.h"
#include "BoxCollider.h"

#include <nlohmann/json.hpp>

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

std::string Engine::Components::BoxCollider::SaveToJson(std::string filePath) {
  nlohmann::json json = nlohmann::json{{"componentType", "boxCollider"},
                                       {"size_x", std::to_string(_size.x)},
                                       {"size_y", std::to_string(_size.y)},
                                       {"size_z", std::to_string(_size.z)},
                                       {"isTrigger", _trigger}};

  std::ofstream ofstream;
  ofstream.open(filePath);
  ofstream << json.dump(4);
  ofstream.close();

  return json.dump(4);
}


