#include "ShadowTarget.h"

#include "Components/UIRenderer.h"

ShadowTarget::ShadowTarget(std::shared_ptr< Engine::Transform > target)
    : _shadowRate(0.0f), _bar(std::make_shared< BarData >()), _maxAmount(100.0f),
      _currentAmount(0.0f), _fillSpeed(10.0f), _target(target), _maxSamplesPassed(0) {
  auto entity     = Engine::ECS::EntityManager::GetInstance().CreateEntity();
  auto renderer   = entity->AddComponent< Engine::Components::UIRenderer >();
  _bar->transform = entity->AddComponent< Engine::Transform >();
  _bar->bar       = std::make_shared< Engine::Renderer::Bar >();
  renderer->GetElements().push_back(_bar->bar);
  _targetID = _target->GetEntityID();
  _modelQuery =
      Engine::ECS::EntityManager::GetComponent< Engine::Components::MeshRenderer >(_targetID)
          ->GetModel()
          ->GetQuery();
}

auto ShadowTarget::OnCreate() -> void {
  _bar->bar->FillRatio(_currentAmount / _maxAmount);
  _bar->bar->Horizontal(false);
  _bar->transform->Position(glm::vec3(400.0f, 200.0f, 0.0f));
  _bar->bar->BackgroundColor(glm::vec4(0.0f, 1.0f, 1.0f, 1.0f));
  _bar->bar->FillColor(glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));
  _bar->bar->Size(glm::vec2(50.0f, 500.0f));
  _bar->bar->Padding(glm::vec2(5.0f, 5.0f));
}

auto ShadowTarget::Update(float deltaTime) -> void {
  _bar->bar->FillRatio(_currentAmount / _maxAmount);
  _currentAmount += _fillSpeed * _shadowRate * deltaTime;
  if (_currentAmount > _maxAmount)
    _currentAmount = _maxAmount;
}

auto ShadowTarget::GetTargetID() -> Engine::ECS::EntityID {
  return _targetID;
}

auto ShadowTarget::GetQuery() -> Engine::GL::QueryObject& {
  return _modelQuery;
}

auto ShadowTarget::ShadowRate() -> float {
  return _shadowRate;
}

auto ShadowTarget::ShadowRate(float shadowRate) -> float {
  _shadowRate = shadowRate;
}

auto ShadowTarget::SamplesPassed(GLint samplesPassed) -> void {
  if (samplesPassed > _maxSamplesPassed)
    _maxSamplesPassed = samplesPassed;

  _shadowRate = (float)samplesPassed / _maxSamplesPassed;
}
