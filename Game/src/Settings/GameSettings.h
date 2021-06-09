#pragma once
#include <memory>

namespace Engine {
  namespace GL {
    class Shader;
  }
}  // namespace Engine

class GameSettings {
private:
  /* Graphics */
  float _brightness = 0.0f;
  float _contrast   = 1.0f;
  float _gamma      = 2.2f;
  /* Time */
  float _gameTimeScale   = 1.0f;
  float _playerTimeScale = 1.0f;

  /* Duration */
  float _playerSpeedUpDuration = 3.0f;

  // shaders to apply changes
  std::shared_ptr< Engine::GL::Shader > _final_pass;
  std::shared_ptr< Engine::GL::Shader > _bar;
  std::shared_ptr< Engine::GL::Shader > _imageUI;
  std::shared_ptr< Engine::GL::Shader > _skybox;
  std::shared_ptr< Engine::GL::Shader > _text;

public:
  GameSettings();//= default;
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

  /* Duration */
  auto PlayerSpeedUpDuration() const noexcept -> float;
};
