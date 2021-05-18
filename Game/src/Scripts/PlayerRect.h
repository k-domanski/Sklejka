#pragma once
#include <Engine.h>
#include <Scripts/PlayerController.h>

class PlayerRect : public Engine::Script {
private:
  std::shared_ptr< Engine::Transform > _transform;
  std::shared_ptr< PlayerController > _playerController;
  std::shared_ptr< Engine::NodeSystem > _nodeSystem;
  std::shared_ptr< Engine::Node > _currentNode;
  std::shared_ptr< Engine::Transform > _nodeTransform;
  glm::vec2 _size{4, 4};
  glm::vec3 _moveVelocity{0};
  float _speed           = 3.0f;
  float _seekSpeed       = 1.0f;
  float _minNodeDistance = 0.2f;

public:
  PlayerRect(const std::shared_ptr< PlayerController >& player_controller);
  auto OnCreate() -> void override;
  auto Update(float deltaTime) -> void override;
  auto Size() const noexcept -> glm::vec2;
  auto Size(const glm::vec2& size) noexcept -> glm::vec2;

private:
  auto HandleInput(float& vertical, float& horizontal, float& roll) -> void;
  auto SeekTarget(float deltaTime) -> void;
  auto HandleMove(float vertical, float horizontal, float deltaTime) -> void;
  auto HandleRotation(float roll, float deltaTime) -> void;
  auto GetNode() -> std::shared_ptr< Engine::Node >;
};
