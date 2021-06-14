#include "pch.h"
#include "ParticleSystem.h"
#include "Components/ParticleEmitter.h"

namespace Engine::Systems {
  ParticleSystem::ParticleSystem() {
    AddSignature< Transform >();
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
        particle.attributes.scale =
            glm::max(glm::vec2(0.0f), particle.attributes.scale - emitter->_sizeDecay * deltaTime);
      }

      SortParticles(emitter);

      /* Spawn new particles */

      if (emitter->_spawnRate != 0.0f) {
        if (emitter->_intervalCache <= 0.0f) {
          if (!SpawnParticle(emitter)) {
            LOG_INFO("Failed to spawn particle");
          }
          emitter->_intervalCache += emitter->_interval;
        } else {
          emitter->_intervalCache -= deltaTime;
        }
      }

      /* Spawn particles until spawn cache < 1.0f*/
      /*emitter->_spawnCache += emitter->_spawnRate * deltaTime;
      while (emitter->_spawnCache >= 1.0f) {
        if (!SpawnParticle(emitter)) {
          break;
        }
        emitter->_spawnCache -= 1.0f;
      }*/

      emitter->UpdateBuffer();
    }
  }
  auto ParticleSystem::SpawnParticle(std::shared_ptr< ParticleEmitter >& emitter) -> bool {
    /* Find dead particle */
    auto it = std::find_if(emitter->_particles.rbegin(), emitter->_particles.rend(),
                           [](ParticleData& data) { return data.lifetime <= 0.0f; });
    if (it == emitter->_particles.rend()) {
      return false;
    }

    auto position = emitter->GetEntity()->GetComponent< Transform >()->WorldPosition();
    /* Prepare particle */
    auto& particle               = *it;
    particle.attributes.scale    = emitter->_scale;
    particle.attributes.position = position;
    particle.lifetime            = emitter->_lifetime;

    if (auto ratio = emitter->_velocityRandomness; ratio > 0.0f) {
      auto vel   = emitter->_velocity;
      auto perp1 = PerpendicularVector(vel);
      auto perp2 = glm::cross(vel, perp1);
      auto dir   = glm::normalize(vel);

      auto ang = ((float)rand() / RAND_MAX) * glm::pi< float >() * 4.0f; /* 2 * the circumference */
      auto on_plane = (glm::cos(ang) * perp1 + glm::sin(ang) * perp2) * ratio;
      auto new_vel  = glm::normalize(dir + on_plane) * glm::vec3(glm::length(vel));

      particle.velociy = new_vel;
    } else {
      particle.velociy = emitter->_velocity;
    }

    return true;
  }
  auto ParticleSystem::SortParticles(std::shared_ptr< ParticleEmitter >& emitter) -> void {
    const auto OrderFunction = [](const auto& lhs, const auto& rhs) -> bool {
      return lhs.lifetime > rhs.lifetime;
    };
    if (!std::is_sorted(emitter->_particles.begin(), emitter->_particles.end(), OrderFunction)) {
      std::sort(emitter->_particles.begin(), emitter->_particles.end(), OrderFunction);
    }
  }
  auto ParticleSystem::PerpendicularVector(glm::vec3 vector) const noexcept -> glm::vec3 {
    auto div_index = 0;
    while (div_index < 3 && glm::abs(vector[div_index]) < 0.001f) {
      ++div_index;
    }
    if (div_index >= 3) {
      return glm::vec3(0.0f);
    }

    float sum = 0.0f;
    glm::vec3 result(1.0f);
    for (int i = 0; i < 3; ++i) {
      if (i == div_index) {
        continue;
      }
      sum += vector[i];
    }
    result[div_index] = sum / vector[div_index];
    return glm::normalize(result);
  }
}  // namespace Engine::Systems
