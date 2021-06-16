#include "ShadowTarget.h"
#include "Engine.h"
#include "GameManager.h"
#include "Components/UIRenderer.h"

ShadowTarget::ShadowTarget(std::shared_ptr< Engine::ECS::Entity > target)
    : _bar(std::make_shared< BarData >()), _maxAmount(1.0f), _currentAmount(0.0f), _target(target),
      _maxSamplesPassed(0) {
  auto entity     = Engine::ECS::EntityManager::GetInstance().CreateEntity();
  auto renderer   = entity->AddComponent< Engine::Components::UIRenderer >();
  _bar->transform = entity->AddComponent< Engine::Transform >();
  _bar->bar       = std::make_shared< Engine::Renderer::Bar >();
  _bar->image       = std::make_shared< Engine::Renderer::Image >();
  renderer->AddElement(_bar->bar);
  renderer->AddElement(_bar->image);
  _rendererSystem =
      Engine::ECS::EntityManager::GetInstance().GetSystem< Engine::Systems::Renderer >();
  _gameTimeLerp.Set(1.0f, 1.0f, 1.0f);
  _playerTimeLerp.Set(1.0f, 1.0f, 1.0f);
  _lerpTime   = 0.5f;
  _lastStateX = false;
  _timeSlowed = false;
}

auto ShadowTarget::OnCreate() -> void {
  _rendererSystem =
      Engine::ECS::EntityManager::GetInstance().GetSystem< Engine::Systems::Renderer >();
  stbi_set_flip_vertically_on_load(true);
  _bar->bar->BackgroundTexture(
      Engine::AssetManager::GetTexture2D("./textures/UI/energy_background_ver.png"));
  _bar->bar->FillTexture(Engine::AssetManager::GetTexture2D("./textures/UI/energy_fill_ver.png"));
  _bar->bar->FillRatio(glm::min(1.0f, _currentAmount / _maxAmount));
  _bar->bar->Horizontal(false);
  _bar->bar->Middle(false);
  _bar->transform->Position(glm::vec3(100.0f, 350.0f, 0.0f));
  _bar->bar->BackgroundColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
  _bar->bar->FillColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
  _bar->bar->Size(glm::vec2(55.0f, 400.0f));
  _bar->bar->Padding(glm::vec2(10.0f, 10.0f));

  _bar->image->Texture(Engine::AssetManager::GetTexture2D("./textures/UI/spowolnienie_ic.png"));
  _bar->image->Size({55.0f, 53.0f});
  _bar->image->Offset({0.0f, 250.0f});
  stbi_set_flip_vertically_on_load(false);
}

auto ShadowTarget::Update(float deltaTime) -> void {
  if (GameManager::IsPaused()) {
    return;
  }
  auto currentStataX = Engine::Input::IsGamepadButtonPressed(Engine::GamepadCode::BUTTON_X);
  if (currentStataX && !_lastStateX) {
    SlowTime();
  }
  _lastStateX = currentStataX;

  const auto player_settings = GameManager::GetPlayerSettings();
  if (!_timeSlowed) {
    if (IsInShadow()) {
      /* Increase energy */
      _currentAmount += (1.0f / player_settings->EnergyFillTime()) * deltaTime;
      _currentAmount = glm::min(_currentAmount, 1.0f);
    }
  } else {
    /* Decrease energy */
    if (_currentAmount > 0.0f) {
      _currentAmount -= (1.0f / (player_settings->SlowTimeDuration() + 2 * _lerpTime)) * deltaTime;
    } else {
      SetTimeSlowed(false);
      GameManager::GetSoundEngine()->play2D("./Assets/sounds/resume_time.wav");
      _currentAmount = 0.0f;
    }
  }
  _bar->bar->FillRatio(glm::min(1.0f, _currentAmount));

  GameManager::GetGameSettings()->GameTimeScale(_gameTimeLerp.Update(deltaTime));
  GameManager::GetGameSettings()->PlayerTimeScale(_playerTimeLerp.Update(deltaTime));
}

auto ShadowTarget::OnKeyPressed(Engine::Key key) -> void {
  if (GameManager::IsPaused()) {
    return;
  }
  if (key == Engine::Key::LEFT_SHIFT) {
    SlowTime();
  }
}

auto ShadowTarget::SlowTime() -> void {
  _timeSlowed = !_timeSlowed;
  SetTimeSlowed(_timeSlowed);

  if (_timeSlowed) {
    GameManager::GetSoundEngine()->play2D("./Assets/sounds/slow_time.wav");
  } else {
    GameManager::GetSoundEngine()->play2D("./Assets/sounds/resume_time.wav");
  }
}

auto ShadowTarget::GetTarget() -> std::shared_ptr< Engine::ECS::Entity > {
  return _target;
}

auto ShadowTarget::SetTimeSlowed(bool value) -> void {
  _timeSlowed  = value;
  auto p_scale = GameManager::GetGameSettings()->PlayerTimeScale();
  auto g_scale = GameManager::GetGameSettings()->GameTimeScale();

  auto pt_start = p_scale;
  auto pt_end   = value ? 0.75 : 1.f;

  auto gt_start = g_scale;
  auto gt_end   = value ? 0.5 : 1.f;

  _playerTimeLerp.Set(pt_start, pt_end, _lerpTime);
  _gameTimeLerp.Set(gt_start, gt_end, _lerpTime);
  GameManager::GetGameSettings()->GameTimeScale(_gameTimeLerp.Value());
  GameManager::GetGameSettings()->PlayerTimeScale(_playerTimeLerp.Value());
}

auto ShadowTarget::IsInShadow() -> bool {
  auto samplesPassed = _rendererSystem->ObjectInShadow(_target);
  _maxSamplesPassed  = 1000;  // this is not working because squirrel is then in its own shadow ->
                              // glm::max(samplesPassed, _maxSamplesPassed);
  auto test_against = glm::max(100.0f, _maxSamplesPassed / 2.0f);
  return samplesPassed < test_against;
}
