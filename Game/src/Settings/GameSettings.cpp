#include "GameSettings.h"

auto GameSettings::Brightness() const noexcept -> float {
  return _brightness;
}

auto GameSettings::Brightness(float value) noexcept -> float {
  return _brightness = value;
}

auto GameSettings::Contrast() const noexcept -> float {
  return _contrast;
}

auto GameSettings::Contrast(float value) noexcept -> float {
  return _contrast = value;
}

auto GameSettings::Gamma() const noexcept -> float {
  return _gamma;
}

auto GameSettings::Gamma(float value) noexcept -> float {
  return _gamma = value;
}

auto GameSettings::GameTimeScale() const noexcept -> float {
  return _gameTimeScale;
}

auto GameSettings::GameTimeScale(float scale) noexcept -> float {
  return _gameTimeScale = scale;
}

auto GameSettings::PlayerTimeScale() const noexcept -> float {
  return _playerTimeScale;
}

auto GameSettings::PlayerTimeScale(float scale) noexcept -> float {
  return _playerTimeScale = scale;
}
