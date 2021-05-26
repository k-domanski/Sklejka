#include "ShadowTarget.h"
#include "Engine.h"
#include "Components/UIRenderer.h"

ShadowTarget::ShadowTarget(Engine::ECS::EntityID target)
    : Script(), _shadowRate(0.0f), _bar(std::make_shared< BarData >()), _maxAmount(100.0f),
      _currentAmount(0.0f), _fillSpeed(10.0f), _targetID(target), _maxSamplesPassed(0) {
  auto entity     = Engine::ECS::EntityManager::GetInstance().CreateEntity();
  auto renderer   = entity->AddComponent< Engine::Components::UIRenderer >();
  _bar->transform = entity->AddComponent< Engine::Transform >();
  _bar->bar       = std::make_shared< Engine::Renderer::Bar >();
  renderer->GetElements().push_back(_bar->bar);
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
  SamplesPassed(_rendererSystem->ObjectInShadow(_targetID));
  _bar->bar->FillRatio(glm::min(1.0f, _currentAmount / _maxAmount));
  _currentAmount += _fillSpeed * _shadowRate * deltaTime;
  if (_currentAmount > _maxAmount)
    // _currentAmount = _maxAmount;
    _currentAmount = 0.0f;
}

auto ShadowTarget::GetTargetID() -> Engine::ECS::EntityID {
  return _targetID;
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
