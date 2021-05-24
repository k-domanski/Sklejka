#include <pch.h>
#include "GameManager.h"

GameManager::GameManager() {
  _gameSettings   = std::make_shared< GameSettings >();
  _playerSettings = std::make_shared< PlayerSettings >();
  _soundEngine    = std::shared_ptr<irrklang::ISoundEngine> (irrklang::createIrrKlangDevice());
}

auto GameManager::Initialize() -> void {
  if (_instance == nullptr) {
    _instance = std::shared_ptr< GameManager >(new GameManager());
  }
}

auto GameManager::GetGameSettings() noexcept -> std::shared_ptr< GameSettings > {
  return _instance->_gameSettings;
}

auto GameManager::GetPlayerSettings() noexcept -> std::shared_ptr< PlayerSettings > {
  return _instance->_playerSettings;
}

auto GameManager::SwitchScene(SceneName scene) -> void {
  //TODO
}

auto GameManager::GetSoundEngine() noexcept -> std::shared_ptr<irrklang::ISoundEngine>
{
  return _instance->_soundEngine;
}
