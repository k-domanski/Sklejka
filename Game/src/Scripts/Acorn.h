#pragma once
#include <Engine.h>
#include "Components/Rigidbody.h"

class Acorn : public Engine::Script< Acorn > {
private:
  glm::vec3 _axis;
  float _rotationSpeed;
  float _gravity;
  float _timeout;
  float _destroyTimeout;
  std::shared_ptr< Engine::Transform > _transform;
  std::shared_ptr< Engine::Components::Rigidbody > _rigidbody;

public:
  Acorn() = default;
  auto OnCreate() -> void override;
  auto Update(float deltaTime) -> void override;
  auto OnCollisionEnter(const std::shared_ptr< Engine::Components::Collider >& collider)
      -> void override;

private:
  auto TimeoutDestroy() -> void;
};