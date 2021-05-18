#pragma once
#include <Engine.h>

class PlayerController : public Engine::Script {
private:
  float _speed             = 2.0f;
  float _minNodeDistance   = 0.2f;
  float _seekSpeed         = 1.0f;
  glm::vec3 _moveVelocity = {0.0f, 0.0f, 1.0f};
  std::shared_ptr< Engine::Transform > _playerTransform;
  std::shared_ptr< Engine::NodeSystem > _nodeSystem;
  std::shared_ptr< Engine::Node > _currentNode;
  std::shared_ptr< Engine::Transform > _nodeTransform;
  float time;

public:
  PlayerController(const std::shared_ptr< Engine::Transform >& player_transform);
  auto OnCreate() -> void override;
  auto Update(float deltaTime) -> void override;

private:
  auto SeekTarget(float deltaTime) -> void;
  auto GetNode() -> std::shared_ptr< Engine::Node >;
};