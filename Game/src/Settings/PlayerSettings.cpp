#include "PlayerSettings.h"

auto PlayerSettings::ForwardSpeed() const noexcept -> float {
  return _forwardSpeed;
}

auto PlayerSettings::ForwardSpeed(float speed) noexcept -> float {
  return _forwardSpeed = speed;
}

auto PlayerSettings::ControlSpeed() const noexcept -> float {
  return _controlSpeed;
}

auto PlayerSettings::ControlSpeed(float speed) noexcept -> float {
  return _controlSpeed = speed;
}

auto PlayerSettings::RollSpeed() const noexcept -> float {
  return _rollSpeed;
}

auto PlayerSettings::RollSpeed(float speed) noexcept -> float {
  return _rollSpeed = speed;
}

auto PlayerSettings::SeekSpeed() const noexcept -> float {
  return _seekSpeed;
}

auto PlayerSettings::SeekSpeed(float speed) noexcept -> float {
  return _seekSpeed = speed;
}

auto PlayerSettings::MinNodeDistance() const noexcept -> float {
  return _minNodeDistance;
}

auto PlayerSettings::MinNodeDistance(float value) noexcept -> float {
  return _minNodeDistance = value;
}

auto PlayerSettings::RectSize() const noexcept -> glm::vec2 {
  return _rectSize;
}

auto PlayerSettings::RectSize(const glm::vec2& size) noexcept -> glm::vec2 {
  return _rectSize = size;
}
