#pragma once
#include <Engine.h>

namespace Engine {
  class ParticleEmitter;
}

namespace Engine::Systems {
  class ParticleSystem : public ECS::System {
  public:
    ParticleSystem();
    auto Update(float deltaTime) -> void override;

  private:
    auto SpawnParticle(std::shared_ptr< ParticleEmitter >& emitter) -> bool;
    auto SortParticles(std::shared_ptr< ParticleEmitter >& emitter) -> void;
  };
}  // namespace Engine::Systems