#include "pch.h"
#include "Collider.h"

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
