#include "ShadowTarget.h"
#include "Engine.h"
#include "GameManager.h"
#include "Components/UIRenderer.h"

ShadowTarget::ShadowTarget(std::shared_ptr< Engine::ECS::Entity > target)
    : Script(), _shadowRate(0.0f), _bar(std::make_shared< BarData >()), _maxAmount(100.0f),
      _currentAmount(0.0f), _fillSpeed(10.0f), _target(target), _maxSamplesPassed(0) {
  auto entity     = Engine::ECS::EntityManager::GetInstance().CreateEntity();
  auto renderer   = entity->AddComponent< Engine::Components::UIRenderer >();
  _bar->transform = entity->AddComponent< Engine::Transform >();
  _bar->bar       = std::make_shared< Engine::Renderer::Bar >();
  renderer->AddElement(_bar->bar);
  _rendererSystem =
      Engine::ECS::EntityManager::GetInstance().GetSystem< Engine::Systems::Renderer >();
}

auto ShadowTarget::OnCreate() -> void {
  _rendererSystem =
      Engine::ECS::EntityManager::GetInstance().GetSystem< Engine::Systems::Renderer >();
  _bar->bar->FillRatio(glm::min(1.0f, _currentAmount / _maxAmount));
  _bar->bar->Horizontal(false);
  _bar->transform->Position(glm::vec3(100.0f, 400.0f, 0.0f));
  _bar->bar->BackgroundColor(glm::vec4(0.0f, 1.0f, 1.0f, 1.0f));
  _bar->bar->FillColor(glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));
  _bar->bar->Size(glm::vec2(25.0f, 300.0f));
  _bar->bar->Padding(glm::vec2(5.0f, 5.0f));
}

auto ShadowTarget::Update(float deltaTime) -> void {
  if (!_timeSlowed) {
    SamplesPassed(_rendererSystem->ObjectInShadow(_target));
    _currentAmount +=
        _fillSpeed * _shadowRate * deltaTime * GameManager::GetGameSettings()->PlayerTimeScale();
  }
  else
  {
    _currentAmount -= 10 * deltaTime * GameManager::GetGameSettings()->PlayerTimeScale();
  }

  if (_currentAmount <= 0)
  {
    _currentAmount = 0;
    if (_timeSlowed)
      SetTimeSlowed(false);
  }

  _bar->bar->FillRatio(glm::min(1.0f, _currentAmount / _maxAmount));
}

auto ShadowTarget::OnKeyPressed(Engine::Key key) -> void
{
  if (key == Engine::Key::Z)
  {
    if (_timeSlowed) {
      SetTimeSlowed(false);
    }
    else if (_currentAmount > 0)
    {
      SetTimeSlowed(true);
    }
  }
}

auto ShadowTarget::GetTarget() -> std::shared_ptr< Engine::ECS::Entity > {
  return _target;
}

auto ShadowTarget::ShadowRate() -> float {
  return _shadowRate;
}

auto ShadowTarget::ShadowRate(float shadowRate) -> void {
  _shadowRate = shadowRate;
}

auto ShadowTarget::SamplesPassed(GLint samplesPassed) -> void {
  samplesPassed = std::clamp(samplesPassed, 0,
                             1000);  // probably shold be a variable in game setting or somewhere
  if (samplesPassed > _maxSamplesPassed)
    _maxSamplesPassed = samplesPassed;
  _shadowRate = (float)samplesPassed / (float)_maxSamplesPassed;
  _shadowRate = 1.0f - _shadowRate;
  _shadowRate = std::clamp(_shadowRate, 0.0f, 1.0f);
}

auto ShadowTarget::SetTimeSlowed(bool value) -> void
{
  _timeSlowed = value;
  GameManager::GetGameSettings()->PlayerTimeScale(value ? 0.25f : 1.f);
}
