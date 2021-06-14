#pragma once
#include <glm/glm.hpp>

class PlayerSettings {
private:
  float _speedMultiplier  = 2.0f;
  float _forwardSpeedBase = 7.5f;
  float _bossForwardSpeedBase = 8.5f;
  float _bossForwardSpeed = 8.5f;
  float _forwardSpeed     = 7.5f; /* Forward Speed (Player Rect) */
  float _controlSpeedBase = 3.2f; /* Left, Right, Up, Down Speed */
  float _controlSpeed     = 3.2f; /* Left, Right, Up, Down Speed */
  float _moveRotationSpeed = 1.25f; 
  float _rollSpeed        = 2.2f; /* Roll speed */
  float _seekSpeed        = 2.0f; /* Direction seek speed */
  float _minNodeDistance  = 4.0f; /* Min distance to the target node */
  float _maxMoveRotation   = 0.22f;
  glm::vec2 _rectSize     = {4.75, 4.5};

public:
  PlayerSettings() = default;
  auto SpeedMultiplier() const noexcept -> float;
  auto ForwardSpeedBase() const noexcept -> float;
  auto BossForwardSpeedBase() const noexcept -> float;
  auto ControlSpeedBase() const noexcept -> float;
  auto ForwardSpeed() const noexcept -> float;
  auto ForwardSpeed(float speed) noexcept -> float;
  auto BossForwardSpeed() const noexcept -> float;
  auto BossForwardSpeed(float speed) noexcept -> float;
  auto ControlSpeed() const noexcept -> float;
  auto ControlSpeed(float speed) noexcept -> float;
  auto MoveRotationSpeed() const noexcept -> float;
  auto MoveRotationSpeed(float speed) noexcept -> float;
  auto MaxMoveRotation() const noexcept -> float;
  auto MaxMoveRotation(float maxRotation) noexcept -> float;
  auto RollSpeed() const noexcept -> float;
  auto RollSpeed(float speed) noexcept -> float;
  auto SeekSpeed() const noexcept -> float;
  auto SeekSpeed(float speed) noexcept -> float;
  auto MinNodeDistance() const noexcept -> float;
  auto MinNodeDistance(float value) noexcept -> float;
  auto RectSize() const noexcept -> glm::vec2;
  auto RectSize(const glm::vec2& size) noexcept -> glm::vec2;
};