#include "Jetpack.h"
#include <GameManager.h>
#include <Components/ParticleEmitter.h>

using namespace Engine;

auto Jetpack::OnCreate() -> void {
  _transform = Entity()->GetComponent< Transform >();
  _speed     = 4.0f;
}

auto Jetpack::Update(float deltaTime) -> void {
  if (_emitters.size() == 0) {
    return;
  }

  const auto time_scale = GameManager::GetGameSettings()->GameTimeScale();
  for (auto& em : _emitters) {
    em->Velocity(_transform->Forward() * _speed);
  }
}

auto Jetpack::SetEmitters(std::vector< std::shared_ptr< Engine::ParticleEmitter > > emitters)
    -> void {
  _emitters = emitters;
}
