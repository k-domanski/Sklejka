#include "pch.h"
#include "ParticleEmitter.h"
#include <Renderer/Material.h>

namespace Engine {
  ParticleEmitter::ParticleEmitter(std::size_t particle_count)
      : ECS::Component("Particle Emitter", ECS::GetComponentTypeID< ParticleEmitter >()) {
    _vertexArray.Bind();
    _instanceBuffer.Bind();
    EnableAttributes();

    _particles.resize(particle_count);
    _attributes.resize(particle_count);
  }
  auto ParticleEmitter::UpdateBuffer() -> void {
    std::transform(_particles.begin(), _particles.end(), _attributes.begin(),
                   [](const ParticleData& p) { return p.attributes; });
    _instanceBuffer.SetData(_attributes.size() * sizeof(ParticleData::Attributes),
                            _attributes.data());
  }
  auto ParticleEmitter::Draw() -> void {
    _material->Use();
    _instanceBuffer.Bind();
    glDrawArraysInstanced(GL_POINTS, 0, 1, _particles.size());
  }
  auto ParticleEmitter::SizeDecay() const noexcept -> float {
    return _sizeDecay;
  }
  auto ParticleEmitter::SizeDecay(float decay) noexcept -> float {
    return _sizeDecay = decay;
  }
  auto ParticleEmitter::Lifetime() const noexcept -> float {
    return _lifetime;
  }
  auto ParticleEmitter::Lifetime(float lifetime) noexcept -> float {
    return _lifetime = lifetime;
  }
  auto ParticleEmitter::SpawnRate() const noexcept -> float {
    return _spawnRate;
  }
  auto ParticleEmitter::SpawnRate(float rate) noexcept -> float {
    return _spawnRate = rate;
  }
  auto ParticleEmitter::Scale() const noexcept -> glm::vec2 {
    return _scale;
  }
  auto ParticleEmitter::Scale(const glm::vec2& scale) noexcept -> glm::vec2 {
    return _scale = scale;
  }
  auto ParticleEmitter::Velocit() const noexcept -> glm::vec3 {
    return _velocity;
  }
  auto ParticleEmitter::Velocity(const glm::vec3& velocity) noexcept -> glm::vec3 {
    return _velocity = velocity;
  }
  auto ParticleEmitter::Material() const noexcept -> std::shared_ptr< Renderer::Material > {
    return _material;
  }
  auto ParticleEmitter::Material(const std::shared_ptr< Renderer::Material >& material) noexcept
      -> std::shared_ptr< Renderer::Material > {
    return _material = material;
  }
  auto ParticleEmitter::EnableAttributes() -> void {
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3),
                          (const void*)offsetof(ParticleData::Attributes, position));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3),
                          (const void*)offsetof(ParticleData::Attributes, color));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2),
                          (const void*)offsetof(ParticleData::Attributes, scale));
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(float),
                          (const void*)offsetof(ParticleData::Attributes, rotation));

    glVertexAttribDivisor(0, 1);
    glVertexAttribDivisor(1, 1);
    glVertexAttribDivisor(2, 1);
    glVertexAttribDivisor(3, 1);
  }
}  // namespace Engine
