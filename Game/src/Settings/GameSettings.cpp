#include "GameSettings.h"

#include "App/AssetManager.h"

GameSettings::GameSettings() {
  _final_pass = Engine::AssetManager::GetShader("./shaders/final_pass.glsl");
  _bar        = Engine::AssetManager::GetShader("./shaders/bar.glsl");
  _imageUI    = Engine::AssetManager::GetShader("./shaders/imageUI.glsl");
  _skybox     = Engine::AssetManager::GetShader("./shaders/skybox.glsl");
  _text       = Engine::AssetManager::GetShader("./shaders/text.glsl");

  Contrast(_contrast);
  Brightness(_brightness);
  Gamma(_gamma);
}

auto GameSettings::Brightness() const noexcept -> float {
  return _brightness;
}

auto GameSettings::Brightness(float value) noexcept -> float {
  _final_pass->Use();
  _final_pass->SetValue("u_Brightness", value);
  _bar->Use();
  _bar->SetValue("u_Brightness", value);
  _imageUI->Use();
  _imageUI->SetValue("u_Brightness", value);
  _skybox->Use();
  _skybox->SetValue("u_Brightness", value);
  _text->Use();
  _text->SetValue("u_Brightness", value);

  return _brightness = value;
}

auto GameSettings::Contrast() const noexcept -> float {
  return _contrast;
}

auto GameSettings::Contrast(float value) noexcept -> float {
  _final_pass->Use();
  _final_pass->SetValue("u_Contrast", value);
  _bar->Use();
  _bar->SetValue("u_Contrast", value);
  _imageUI->Use();
  _imageUI->SetValue("u_Contrast", value);
  _skybox->Use();
  _skybox->SetValue("u_Contrast", value);
  _text->Use();
  _text->SetValue("u_Contrast", value);
  return _contrast = value;
}

auto GameSettings::Gamma() const noexcept -> float {
  return _gamma;
}

auto GameSettings::Gamma(float value) noexcept -> float {
  _final_pass->Use();
  _final_pass->SetValue("u_Gamma", value);
  _bar->Use();
  _bar->SetValue("u_Gamma", value);
  _imageUI->Use();
  _imageUI->SetValue("u_Gamma", value);
  _skybox->Use();
  _skybox->SetValue("u_Gamma", value);
  _text->Use();
  _text->SetValue("u_Gamma", value);
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

auto GameSettings::PlayerSpeedUpDuration() const noexcept -> float {
  return _playerSpeedUpDuration;
}
