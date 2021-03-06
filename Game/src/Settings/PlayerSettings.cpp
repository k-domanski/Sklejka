#include "PlayerSettings.h"

auto PlayerSettings::SpeedMultiplier() const noexcept -> float {
  return _speedMultiplier;
}

auto PlayerSettings::BossSpeedMultiplier() const noexcept -> float {
  return _bossSpeedMultiplier;
}

auto PlayerSettings::ForwardSpeedBase() const noexcept -> float {
  return _forwardSpeedBase;
}

auto PlayerSettings::BossForwardSpeedBase() const noexcept -> float {
  return _bossForwardSpeedBase;
}

auto PlayerSettings::ControlSpeedBase() const noexcept -> float {
  return _controlSpeedBase;
}

auto PlayerSettings::ForwardSpeed() const noexcept -> float {
  return _forwardSpeed;
}

auto PlayerSettings::ForwardSpeed(float speed) noexcept -> float {
  return _forwardSpeed = speed;
}

auto PlayerSettings::BossForwardSpeed() const noexcept -> float {
  return _bossForwardSpeed;
}

auto PlayerSettings::BossForwardSpeed(float speed) noexcept -> float {
  return _bossForwardSpeed = speed;
}

auto PlayerSettings::ControlSpeed() const noexcept -> float {
  return _controlSpeed;
}

auto PlayerSettings::ControlSpeed(float speed) noexcept -> float {
  return _controlSpeed = speed;
}

auto PlayerSettings::MoveRotationSpeed() const noexcept -> float {
  return _moveRotationSpeed;
}

auto PlayerSettings::MoveRotationSpeed(float speed) noexcept -> float {
  return _moveRotationSpeed = speed;
}

auto PlayerSettings::MaxMoveRotation() const noexcept -> float {
  return _maxMoveRotation;
}

auto PlayerSettings::MaxMoveRotation(float maxRotation) noexcept -> float {
  return _maxMoveRotation = maxRotation;
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

auto PlayerSettings::ThrowDistance() const noexcept -> float {
  return _throwDistance;
}

auto PlayerSettings::SlowTimeDuration() const noexcept -> float {
  return _slowTimeDuration;
}

auto PlayerSettings::EnergyFillTime() const noexcept -> float {
  return _energyFillTime;
}
