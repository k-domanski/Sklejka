#pragma once
//#include <Engine.h>

#include "Renderer/Bar.h"
//#include "Renderer/Image.h"
#include "Components/Transform.h"
#include "Engine/Script.h"
#include "Renderer/Text.h"

struct BarData {
  std::shared_ptr< Engine::Transform > transform;
  std::shared_ptr< Engine::Renderer::Bar > bar;
  std::shared_ptr< Engine::Renderer::Text > text;
};

class ShadowTarget : public Engine::Script {
public:
  ShadowTarget(std::shared_ptr< Engine::Transform > target);
  auto OnCreate() -> void override;
  auto Update(float deltaTime) -> void override;
  auto GetTargetID() -> Engine::ECS::EntityID;
  auto GetQuery() -> Engine::GL::QueryObject&;
  auto ShadowRate() -> float;
  auto ShadowRate(float shadowRate) -> void;
  auto SamplesPassed(GLint samplesPassed) -> void;
private:
  std::shared_ptr< Engine::Transform > _target;
  Engine::ECS::EntityID _targetID;
  float _shadowRate;
  float _maxAmount;
  float _currentAmount;
  float _fillSpeed;
  GLint _maxSamplesPassed;
  std::shared_ptr< BarData > _bar;
  Engine::GL::QueryObject _modelQuery;
};
