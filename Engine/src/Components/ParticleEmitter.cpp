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
    _maxParticles = particle_count;
  }
  auto ParticleEmitter::UpdateBuffer() -> void {
    std::transform(_particles.begin(), _particles.end(), _attributes.begin(),
                   [](const ParticleData& p) { return p.attributes; });
    _instanceBuffer.SetData(_attributes.size() * sizeof(ParticleData::Attributes),
                            _attributes.data());
  }
  auto ParticleEmitter::Draw() -> void {
    if (_material == nullptr) {
      return;
    }
    _material->Use();
    _vertexArray.Bind();
    glDrawArraysInstanced(GL_POINTS, 0, 1, _emitCount);
  }
  auto ParticleEmitter::MaxParticles() const noexcept -> std::size_t {
    return _maxParticles;
  }
  auto ParticleEmitter::EmitCount() const noexcept -> std::size_t {
    return _emitCount;
  }
  auto ParticleEmitter::EmitCount(std::size_t count) noexcept -> std::size_t {
    return _emitCount = count;
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
  auto ParticleEmitter::Velocity() const noexcept -> glm::vec3 {
    return _velocity;
  }
  auto ParticleEmitter::Velocity(const glm::vec3& velocity) noexcept -> glm::vec3 {
    return _velocity = velocity;
  }
  auto ParticleEmitter::VelocityRandomness() const noexcept -> float {
    return _velocityRandomness;
  }
  auto ParticleEmitter::VelocityRandomness(float value) noexcept -> float {
    return _velocityRandomness = value;
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

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ParticleData::Attributes),
                          (const void*)offsetof(ParticleData::Attributes, position));
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(ParticleData::Attributes),
                          (const void*)offsetof(ParticleData::Attributes, scale));

    glVertexAttribDivisor(0, 1);
    glVertexAttribDivisor(1, 1);
  }
}  // namespace Engine
