#pragma once
#include <pch.h>
#include "Settings/PlayerSettings.h"
#include "Settings/GameSettings.h"

BETTER_ENUM(__SceneName, int, MainMenu, LVL_1);
typedef __SceneName SceneName;

class GameManager {
private:
  inline static std::shared_ptr< GameManager > _instance = nullptr;

private:
  std::shared_ptr< GameSettings > _gameSettings;
  std::shared_ptr< PlayerSettings > _playerSettings;
  GameManager();

public:
  static auto Initialize() -> void;
  static auto GetGameSettings() noexcept -> std::shared_ptr< GameSettings >;
  static auto GetPlayerSettings() noexcept -> std::shared_ptr< PlayerSettings >;
  static auto SwitchScene(SceneName scene) -> void;
};
