#pragma once

class GameSettings {
private:
  /* Graphics */
  float _brightness = 1.0f;
  float _contrast   = 1.0f;
  float _gamma      = 2.2f;
  /* Time */
  float _gameTimeScale   = 1.0f;
  float _playerTimeScale = 1.0f;

public:
  GameSettings() = default;
  /* Graphics */
  auto Brightness() const noexcept -> float;
  auto Brightness(float value) noexcept -> float;
  auto Contrast() const noexcept -> float;
  auto Contrast(float value) noexcept -> float;
  auto Gamma() const noexcept -> float;
  auto Gamma(float value) noexcept -> float;
  /* Time */
  auto GameTimeScale() const noexcept -> float;
  auto GameTimeScale(float scale) noexcept -> float;
  auto PlayerTimeScale() const noexcept -> float;
  auto PlayerTimeScale(float scale) noexcept -> float;
};