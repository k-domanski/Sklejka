#include "pch.h"
#include "Collider.h"

#include <nlohmann/json.hpp>

Engine::Utility::GJK::Simplex Engine::Components::Collider::get_simplex() const {
  return _simplex;
}

void Engine::Components::Collider::set_simplex(const Utility::GJK::Simplex& simplex) {
  _simplex = simplex;
}

glm::vec3 Engine::Components::Collider::get_center() const {
  return _center;
}

void Engine::Components::Collider::set_center(const glm::vec3& center) {
  _center = center;
}

glm::vec3 Engine::Components::Collider::get_size() const {
  return _size;
}

void Engine::Components::Collider::set_size(const glm::vec3& size) {
  _size = size;
}

bool Engine::Components::Collider::is_trigger() const {
  return _trigger;
}

void Engine::Components::Collider::set_trigger(bool trigger) {
  _trigger = trigger;
}

bool Engine::Components::Collider::is_static() const {
  return _static;
}

void Engine::Components::Collider::set_static(bool static_) {
  _static = static_;
}

Engine::Components::ColliderType Engine::Components::Collider::get_type() const {
  return _type;
}

void Engine::Components::Collider::set_type(ColliderType type) {
  _type = type;
}

std::string Engine::Components::Collider::SaveToJson(std::string filePath)
{
  nlohmann::json json = nlohmann::json{
      {"componentType", "collider"},
      {"colliderType", _type},
      {"trigger", _trigger},
      {"static", _static},
      {"center", {{"x", _center.x}, {"y", _center.y}, {"z", _center.z}}},
      {"size", {{"x", _size.x}, {"y", _size.y}, {"z", _size.z}}}
  };

  std::ofstream ofstream;
  ofstream.open(filePath);
  ofstream << json.dump(4);
  ofstream.close();

  return json.dump(4);
}
