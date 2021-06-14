#pragma once
#include <pch.h>
#include "Settings/PlayerSettings.h"
#include "Settings/GameSettings.h"
#include <GUI/LoadingScreen.h>
#include <GUI/Cutscene.h>
#include "irrKlang.h"
#include "GUI/EndLevelMenu.h"
#include "GUI/LevelSelection.h"
#include "GUI/MainMenu.h"
#include "GUI/OptionsMenu.h"
#include "GUI/PauseMenu.h"

BETTER_ENUM(__SceneName, int, MainMenu, Options, Cutscene, LevelSelection, LVL_1);
typedef __SceneName SceneName;

class GameManager {
private:
  inline static std::shared_ptr< GameManager > _instance = nullptr;

private:
  /* Settigns */
  std::shared_ptr< GameSettings > _gameSettings;
  std::shared_ptr< PlayerSettings > _playerSettings;
  /* -=-=-=-=- */

  /* State */
  SceneName _currentSceneName = SceneName::MainMenu;
  /* -=-=- */

  /* Panels */
  std::shared_ptr< Cutscene > _cutscene;
  std::shared_ptr< MainMenu > _mainMenu;
  std::shared_ptr< LevelSelection > _levelSelection;
  std::shared_ptr< OptionsMenu > _options;
  std::shared_ptr< PauseMenu > _pauseMenu;
  std::shared_ptr< EndLevelMenu > _endLevelMenu;
  /* -=-=-=- */

  /* Scene References */
  inline static std::shared_ptr< Engine::ECS::Entity > _playerRect = nullptr;
  inline static std::shared_ptr< Engine::ECS::Entity > _player     = nullptr;
  inline static std::shared_ptr< Engine::ECS::Entity > _model      = nullptr;
  inline static std::vector< std::shared_ptr< Engine::Transform > > _bellsTransform;
  /* -=-=-=-=-=-=-=-=- */

  /* Resources */
  std::shared_ptr< irrklang::ISoundEngine > _soundEngine;
  std::shared_ptr< Engine::GL::Shader > _fishEyeShader;
  std::shared_ptr< Engine::Renderer::Material > _bellOutlineMaterial;
  /* -=-=-=-=- */

  /* Parameters */
  float _speedUpDuration = 0.0f;
  float _speedFactor     = 1.0f;
  int _frameWaitCounter  = 0;
  /* -=-=-=-=- */

  GameManager();
  static auto SetupPlayer(std::shared_ptr< Engine::Scene >& scene) -> void;
  static auto FindBells() -> void;
  static auto UpdateMarkerColor() -> void;

public:
  static auto Initialize() -> void;
  static auto GetGameSettings() noexcept -> std::shared_ptr< GameSettings >;
  static auto GetPlayerSettings() noexcept -> std::shared_ptr< PlayerSettings >;
  static auto GetSoundEngine() noexcept -> std::shared_ptr< irrklang::ISoundEngine >;
  static auto SwitchScene(SceneName scene) -> void;
  static auto GetScene(SceneName scene) -> std::shared_ptr< Engine::Scene >;
  static auto GetNextSceneName() -> SceneName;
  static auto Update(float deltaTime) -> void;
  static auto PlayerSpeedUp() -> void;
  static auto ShowLevelSumUp(float time, bool win) -> void;
  static auto GetCurrentPlayer() -> std::shared_ptr< Engine::ECS::Entity >;
  static auto KillPlayer() -> void;
  static auto Win() -> void;
  static auto IsGameplayState() -> bool;
  static auto IsGameplayScene() -> bool;
  static auto IsGameplayInitialized() -> bool;

private:
  auto UpdateImpl(float deltaTime) -> void;
  auto PlayCutscene() -> void;
  auto CreatePlayer() -> void;
  auto CreateBoss() -> void;
  auto SetupScripts() -> void;
  auto NextFrameTrigger() -> void;
  auto KillPlayerImpl() -> void;
  auto WinImpl() -> void;
};
