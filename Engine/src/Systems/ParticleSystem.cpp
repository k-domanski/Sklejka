#include "pch.h"
#include "ParticleSystem.h"
#include "Components/ParticleEmitter.h"

namespace Engine::Systems {
  ParticleSystem::ParticleSystem() {
    AddSignature< ParticleEmitter >();
  }
  auto ParticleSystem::Update(float deltaTime) -> void {
    for (auto& entity : _entities) {
      auto emitter = entity->GetComponent< ParticleEmitter >();
      auto p_count = emitter->_particles.size();
      /* Update existing */
      for (int i = 0; i < p_count; ++i) {
        auto& particle = emitter->_particles[i];
        particle.lifetime -= deltaTime;
        /* If life ended - omit */
        if (particle.lifetime <= 0.0f) {
          particle.attributes.scale = {0.0f, 0.0f};
          continue;
        }
        /* Update position and scale */
        particle.attributes.position += particle.velociy * deltaTime;
        particle.attributes.scale *=
            glm::max(glm::vec2(0.0f), (glm::vec2(1.0f) - glm::vec2(emitter->_sizeDecay)))
            * deltaTime;
      }

      SortParticles(emitter);

      /* Spawn new particles */
      emitter->_spawnCache += emitter->_spawnRate * deltaTime;
      /* Spawn particles until spawn cache < 1.0f*/
      while (emitter->_spawnCache >= 1.0f) {
        SpawnParticle(emitter);
        emitter->_spawnCache -= 1.0f;
      }
    }
  }
  auto ParticleSystem::SpawnParticle(std::shared_ptr< ParticleEmitter >& emitter) -> bool {
    /* Find dead particle */
    auto it = std::find_if(emitter->_particles.begin(), emitter->_particles.end(),
                           [](ParticleData& data) { return data.lifetime <= 0.0f; });
    if (it == emitter->_particles.end()) {
      return false;
    }

    /* Prepare particle */
    auto& particle            = *it;
    particle.attributes.scale = emitter->_scale;
    particle.velociy          = emitter->_velocity;
    particle.lifetime         = emitter->_lifetime;

    return true;
  }
  auto ParticleSystem::SortParticles(std::shared_ptr< ParticleEmitter >& emitter) -> void {
    const auto OrderFunction = [](const auto& lhs, const auto& rhs) -> bool {
      return lhs.lifetime > rhs.lifetime;
    };
    std::sort(emitter->_particles.begin(), emitter->_particles.end(), OrderFunction);
  }
}  // namespace Engine::Systems
