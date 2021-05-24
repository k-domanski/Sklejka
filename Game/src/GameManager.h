#pragma once
#include <pch.h>
#include "Settings/PlayerSettings.h"
#include "Settings/GameSettings.h"
#include <GUI/LoadingScreen.h>
#include "irrKlang.h"

BETTER_ENUM(__SceneName, int, MainMenu, Loading, LVL_1);
typedef __SceneName SceneName;

class GameManager {
private:
  inline static std::shared_ptr< GameManager > _instance = nullptr;

private:
  std::shared_ptr< GameSettings > _gameSettings;
  std::shared_ptr< PlayerSettings > _playerSettings;
  std::shared_ptr< LoadingScreen > _loadingScreen;
  std::shared_ptr< irrklang::ISoundEngine > _soundEngine;
  SceneName _currentSceneName = SceneName::MainMenu;
  GameManager();

public:
  static auto Initialize() -> void;
  static auto GetGameSettings() noexcept -> std::shared_ptr< GameSettings >;
  static auto GetPlayerSettings() noexcept -> std::shared_ptr< PlayerSettings >;
  static auto GetSoundEngine() noexcept -> std::shared_ptr< irrklang::ISoundEngine >;
  static auto SwitchScene(SceneName scene) -> void;
  static auto GetScene(SceneName scene) -> std::shared_ptr< Engine::Scene >;
  static auto ShowLoadingScreen() -> void;
};