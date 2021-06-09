#pragma once
#include <Engine.h>
#include <Scripts/PlayerController.h>
#include <Settings/PlayerSettings.h>

#include "Settings/GameSettings.h"

class PlayerRect : public Engine::Script {
private:
  std::shared_ptr< Engine::Transform > _transform;
  std::shared_ptr< Engine::Transform > _modelTransform;
  std::shared_ptr< PlayerController > _playerController;
  std::shared_ptr< Engine::NodeSystem > _nodeSystem;
  std::shared_ptr< Engine::Node > _currentNode;
  std::shared_ptr< Engine::Transform > _nodeTransform;
  glm::vec3 _moveVelocity{0};
  std::shared_ptr< PlayerSettings > _playerSettings;
  std::shared_ptr< GameSettings > _gameSettings;
  bool _canMove;

  /*glm::vec2 _size{4, 4};
  float _speed           = 3.0f;
  float _seekSpeed       = 1.0f;
  float _minNodeDistance = 0.2f;*/

public:
  PlayerRect(const std::shared_ptr< PlayerController >& player_controller, const std::shared_ptr< Engine::Transform>& player_model );
  auto OnCreate() -> void override;
  auto Update(float deltaTime) -> void override;
  auto OnKeyPressed(Engine::Key key) -> void override;
  auto CanMove() -> bool;
  auto CanMove(bool value) -> void;
  // auto Size() const noexcept -> glm::vec2;
  // auto Size(const glm::vec2& size) noexcept -> glm::vec2;

private:
  auto HandleInput(float& vertical, float& horizontal, float& roll) -> void;
  auto SeekTarget(float deltaTime) -> void;
  auto HandleMove(float vertical, float horizontal, float deltaTime) -> void;
  auto HandleRotation(float roll, float deltaTime, glm::vec3 axis = {0.0f, 0.0f, -1.0f}) -> void;
  auto HandleModelRotation(float roll, float deltaTime, glm::vec3 axis = {0.0f, 0.0f, -1.0f}) -> void;
  auto LerpResetRotation() -> void;
  auto GetNode() -> std::shared_ptr< Engine::Node >;
};
