#pragma once
#include <pch.h>
#include "Settings/PlayerSettings.h"
#include "Settings/GameSettings.h"
#include <GUI/LoadingScreen.h>
#include <GUI/Cutscene.h>
#include "irrKlang.h"
#include "GUI/CreditsMenu.h"
#include "GUI/EndLevelMenu.h"
#include "GUI/LevelSelection.h"
#include "GUI/MainMenu.h"
#include "GUI/OptionsMenu.h"
#include "GUI/PauseMenu.h"
#include "GUI/TutorialMenu.h"
#include "Utility/Utility.h"

using Engine::Utility::FloatLerp;

BETTER_ENUM(__SceneName, int, MainMenu, Options, Credits, Cutscene, LevelSelection, LVL_1, LVL_2);
typedef __SceneName SceneName;

BETTER_ENUM(__GameState, int, MainMenu, Gameplay, Paused);
typedef __GameState GameState;

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
  GameState _currentGameState = GameState::Gameplay;
  bool _isPaused{false};
  /* -=-=- */

  /* Soudns */
  irrklang::ISound* _bgLoopSound = nullptr;
  std::vector< irrklang::ISound* > _sceneSounds;
  /* -=-=-=-*/

  /* Panels */
  std::shared_ptr< OptionsMenu > _options;
  std::shared_ptr< Cutscene > _cutscene;
  std::shared_ptr< MainMenu > _mainMenu;
  std::shared_ptr< LevelSelection > _levelSelection;
  std::shared_ptr< CreditsMenu > _creditsMenu;
  std::shared_ptr< TutorialMenu > _tutorialMenu;
  std::shared_ptr< PauseMenu > _pauseMenu;
  std::shared_ptr< EndLevelMenu > _endLevelMenu;
  /* -=-=-=- */

  /* Scene References */
  inline static std::shared_ptr< Engine::ECS::Entity > _playerRect = nullptr;
  inline static std::shared_ptr< Engine::ECS::Entity > _player     = nullptr;
  inline static std::shared_ptr< Engine::ECS::Entity > _model      = nullptr;
  inline static std::vector< std::shared_ptr< Engine::Transform > > _bellsTransform;
  /* -=-=-=-=-=-=-=-=- */

  int _hittedBells;
  bool _BpressedLastFrame;

  //Delay stuff
  std::function< void() > _delayedAction;
  float _delayTimer;
  bool _delayAction;

  /* Resources */
  std::shared_ptr< irrklang::ISoundEngine > _soundEngine;
  std::shared_ptr< irrklang::ISoundEngine > _musicEngine;
  std::shared_ptr< Engine::GL::Shader > _fishEyeShader;
  std::shared_ptr< Engine::GL::Shader > _aberrationShader;
  std::shared_ptr< Engine::Renderer::Material > _bellOutlineMaterial;
  /* -=-=-=-=- */

  /* Parameters */
  float _speedUpDuration = 0.0f;
  float _speedFactor     = 1.0f;
  int _frameWaitCounter  = 0;
  FloatLerp _speedLerp;
  /* -=-=-=-=- */

  GameManager();
  static auto SetupPlayer(std::shared_ptr< Engine::Scene >& scene) -> void;
  static auto FindBells() -> void;
  static auto SetupBGMusic(GameState state) -> void;
  static auto ProcessBell(const std::shared_ptr< Engine::ECS::Entity >& bell) -> void;
  static auto UpdateMarkerColor() -> void;
  static auto CleanSounds() -> void;

public:
  static auto Initialize() -> void;
  static auto GetGameSettings() noexcept -> std::shared_ptr< GameSettings >;
  static auto GetPlayerSettings() noexcept -> std::shared_ptr< PlayerSettings >;
  static auto GetSoundEngine() noexcept -> std::shared_ptr< irrklang::ISoundEngine >;
  static auto GetMusicEngine() noexcept -> std::shared_ptr< irrklang::ISoundEngine >;
  static auto SetVolume(std::shared_ptr< irrklang::ISoundEngine > engine, float value) -> void;
  static auto SwitchScene(SceneName scene) -> void;
  static auto GetScene(SceneName scene) -> std::shared_ptr< Engine::Scene >;
  static auto GetNextSceneName() -> SceneName;
  static auto GetCurrentSceneName() -> SceneName;
  static auto Update(float deltaTime) -> void;
  static auto PlayerSpeedUp() -> void;
  static auto ShowLevelSumUp(bool win, float time, int bells) -> void;
  static auto ShowOptions(std::function< void() > returnFunc) -> void;
  static auto HideOptions() -> void;
  static auto ShowTutorial(std::function< void() > returnFunc) -> void;
  static auto HideTutorial() -> void;
  static auto GetCurrentPlayer() -> std::shared_ptr< Engine::ECS::Entity >;
  static auto GetCurrentPlayerModel() -> std::shared_ptr< Engine::ECS::Entity >;
  static auto KillPlayer() -> void;
  static auto Win() -> void;
  static auto CreateSecondWeasel() -> void;
  static auto IsGameplayState() -> bool;
  static auto IsGameplayScene() -> bool;
  static auto IsGameplayInitialized() -> bool;
  static auto IsPaused() -> bool;
  static auto SetPaused(bool value) -> bool;
  static auto Time() -> float;
  static auto CreateAcorn() -> std::shared_ptr< Engine::ECS::Entity >;
  static auto AddSound(irrklang::ISound* sound) -> void;
  static auto Delay(std::function< void() > action, float time) -> void;

private:
  auto UpdateImpl(float deltaTime) -> void;
  auto PlayCutscene() -> void;
  auto CreatePlayer() -> void;
  auto CreateBoss() -> void;
  auto CreateSecondWeaselImpl() -> void;
  auto PrepareJetpack(std::shared_ptr< Engine::ECS::Entity > jetpack) -> void;
  auto SetupScripts() -> void;
  auto NextFrameTrigger() -> void;
  auto KillPlayerImpl() -> void;
  auto WinImpl() -> void;
};
