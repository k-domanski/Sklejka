#pragma once
#include <glm/glm.hpp>

class PlayerSettings {
private:
  float _forwardSpeed    = 3.0f; /* Forward Speed (Player Rect) */
  float _controlSpeed    = 1.0f; /* Left, Right, Up, Down Speed */
  float _rollSpeed       = 1.0f; /* Roll speed */
  float _seekSpeed       = 1.0f; /* Direction seek speed */
  float _minNodeDistance = 0.2f; /* Min distance to the target node */
  glm::vec2 _rectSize    = {4, 4};

public:
  PlayerSettings() = default;
  auto ForwardSpeed() const noexcept -> float;
  auto ForwardSpeed(float speed) noexcept -> float;
  auto ControlSpeed() const noexcept -> float;
  auto ControlSpeed(float speed) noexcept -> float;
  auto RollSpeed() const noexcept -> float;
  auto RollSpeed(float speed) noexcept -> float;
  auto SeekSpeed() const noexcept -> float;
  auto SeekSpeed(float speed) noexcept -> float;
  auto MinNodeDistance() const noexcept -> float;
  auto MinNodeDistance(float value) noexcept -> float;
  auto RectSize() const noexcept -> glm::vec2;
  auto RectSize(const glm::vec2& size) noexcept -> glm::vec2;
};