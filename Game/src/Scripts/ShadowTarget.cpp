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
  renderer->AddElement(_bar->bar);
  _rendererSystem =
      Engine::ECS::EntityManager::GetInstance().GetSystem< Engine::Systems::Renderer >();
  _gameTimeLerp.Set(1.0f, 1.0f, 1.0f);
  _playerTimeLerp.Set(1.0f, 1.0f, 1.0f);
  _lerpTime = 0.5f;
}

auto ShadowTarget::OnCreate() -> void {
  _rendererSystem =
      Engine::ECS::EntityManager::GetInstance().GetSystem< Engine::Systems::Renderer >();
  _bar->bar->FillRatio(glm::min(1.0f, _currentAmount / _maxAmount));
  _bar->bar->Horizontal(true);
  _bar->bar->Middle(true);
  _bar->transform->Position(glm::vec3(800.0f, 50.0f, 0.0f));
  _bar->bar->BackgroundColor(glm::vec4(0.0f, 1.0f, 1.0f, 1.0f));
  _bar->bar->FillColor(glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));
  _bar->bar->Size(glm::vec2(300.0f, 25.0f));
  _bar->bar->Padding(glm::vec2(5.0f, 5.0f));
}

auto ShadowTarget::Update(float deltaTime) -> void {
  if (GameManager::IsPaused()) {
    return;
  }

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
      _currentAmount = 0.0f;
    }
  }
  _bar->bar->FillRatio(glm::min(0.01f, _currentAmount));

  GameManager::GetGameSettings()->GameTimeScale(_gameTimeLerp.Update(deltaTime));
  GameManager::GetGameSettings()->PlayerTimeScale(_playerTimeLerp.Update(deltaTime));
}

auto ShadowTarget::OnKeyPressed(Engine::Key key) -> void {
  if (key == Engine::Key::LEFT_SHIFT) {
    SlowTime();
  }
}

auto ShadowTarget::SlowTime() -> void {
  if (_timeSlowed) {
    return;
  }
  if (_currentAmount < 1.0f) {
    return;
  }
  SetTimeSlowed(true);
}

auto ShadowTarget::GetTarget() -> std::shared_ptr< Engine::ECS::Entity > {
  return _target;
}

auto ShadowTarget::SetTimeSlowed(bool value) -> void {
  _timeSlowed = value;
  _playerTimeLerp.Set((!value ? 0.75f : 1.f), (value ? 0.75f : 1.f), _lerpTime);
  _gameTimeLerp.Set((!value ? 0.5f : 1.f), (value ? 0.5f : 1.f), _lerpTime);
  GameManager::GetGameSettings()->GameTimeScale(_gameTimeLerp.Value());
  GameManager::GetGameSettings()->PlayerTimeScale(_playerTimeLerp.Value());
}

auto ShadowTarget::IsInShadow() -> bool {
  auto samplesPassed = _rendererSystem->ObjectInShadow(_target);
  _maxSamplesPassed  = glm::max(samplesPassed, _maxSamplesPassed);
  auto test_against  = glm::max(100.0f, _maxSamplesPassed / 2.0f);
  return samplesPassed < test_against;
}
