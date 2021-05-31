#pragma once
#include <Engine.h>

class CollisionDetector : public Engine::Script {
private:
  float _timeoutDuration = 1.0f;
  float _timeout         = 0.0f;

public:
  auto Update(float deltaTime) -> void override;
  auto OnKeyPressed(Engine::Key key) -> void override;
  auto OnCollisionEnter(const std::shared_ptr< Engine::Components::Collider >& collider) -> void;
};