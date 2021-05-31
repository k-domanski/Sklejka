#pragma once
#include <Engine.h>

#include "Renderer/Bar.h"
//#include "Renderer/Image.h"
#include "Components/Transform.h"
#include "Engine/Script.h"
#include "Renderer/Text.h"
#include "Systems/Renderer.h"

struct BarData {
  std::shared_ptr< Engine::Transform > transform;
  std::shared_ptr< Engine::Renderer::Bar > bar;
  std::shared_ptr< Engine::Renderer::Text > text;
};

class ShadowTarget : public Engine::Script {
public:
  ShadowTarget(std::shared_ptr< Engine::ECS::Entity > target);
  auto OnCreate() -> void override;
  auto Update(float deltaTime) -> void override;
  auto OnKeyPressed(Engine::Key key) -> void override;
  auto GetTarget() -> std::shared_ptr< Engine::ECS::Entity >;
  auto ShadowRate() -> float;
  auto ShadowRate(float shadowRate) -> void;
  auto SamplesPassed(GLint samplesPassed) -> void;
  auto SetTimeSlowed(bool value) -> void;

private:
  std::shared_ptr<Engine::ECS::Entity> _target;
  float _shadowRate;
  float _maxAmount;
  float _currentAmount;
  float _fillSpeed;
  bool _timeSlowed;
  GLint _maxSamplesPassed;
  std::shared_ptr< BarData > _bar;
  std::shared_ptr< Engine::Systems::Renderer > _rendererSystem;
};
