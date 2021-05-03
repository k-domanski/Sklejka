#pragma once
#include <Engine.h>

class PlayerController : public Engine::Script {
private:
  float _speed = 0.1f;
  std::shared_ptr< Engine::Transform > _playerTransform;

public:
  PlayerController(const std::shared_ptr< Engine::Transform >& player_transform);
  auto OnCreate() -> void override;
  auto Update(float deltaTime) -> void override;
};