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
    auto PerpendicularVector(glm::vec3 vector) const noexcept -> glm::vec3;
  };
}  // namespace Engine::Systems