#pragma once
#include <Engine.h>

class Jetpack : public Engine::Script< Jetpack > {
private:
  float _speed;
  std::shared_ptr< Engine::Transform > _transform;
  std::vector< std::shared_ptr< Engine::ParticleEmitter > > _emitters;

public:
  auto OnCreate() -> void override;
  auto Update(float deltaTime) -> void override;
  auto SetEmitters(std::vector< std::shared_ptr< Engine::ParticleEmitter > > emitters) -> void;
};