#pragma once
#include "Engine.h"

namespace Engine {
  namespace Systems {
    class ParticleSystem;
  }

  struct ParticleData {
    /* Attributes */
    struct Attributes {
      glm::vec3 position{0.0f};
      glm::vec3 color{1.0f};
      glm::vec2 scale{1.0f};
      float rotation{0.0f};
    } attributes;

    /* System Data */
    glm::vec3 velociy{0};
    float lifetime{0};
    float rotationSpeed{0};
  };

  class ParticleEmitter : public ECS::Component {
    friend class Engine::Systems::ParticleSystem;

  private:
    GL::VertexArray _vertexArray;
    GL::InstanceBuffer _instanceBuffer;
    std::vector< ParticleData > _particles;
    std::vector< ParticleData::Attributes > _attributes;
    float _sizeDecay{0.0f};
    float _lifetime{1.0f};
    float _spawnRate{1.0f};
    glm::vec2 _scale{1.0f, 1.0f};
    glm::vec3 _velocity{0.0f};
    float _spawnCache{0.0f};

    std::shared_ptr< Renderer::Material > _material;

  public:
    ParticleEmitter(std::size_t particle_count);
    auto UpdateBuffer() -> void;
    auto Draw() -> void;
    auto SizeDecay() const noexcept -> float;
    auto SizeDecay(float decay) noexcept -> float;
    auto Lifetime() const noexcept -> float;
    auto Lifetime(float lifetime) noexcept -> float;
    auto SpawnRate() const noexcept -> float;
    auto SpawnRate(float rate) noexcept -> float;
    auto Scale() const noexcept -> glm::vec2;
    auto Scale(const glm::vec2& scale) noexcept -> glm::vec2;
    auto Velocit() const noexcept -> glm::vec3;
    auto Velocity(const glm::vec3& velocity) noexcept -> glm::vec3;
    auto Material() const noexcept -> std::shared_ptr< Renderer::Material >;
    auto Material(const std::shared_ptr< Renderer::Material >& material) noexcept
        -> std::shared_ptr< Renderer::Material >;

  private:
    auto EnableAttributes() -> void;
  };
}  // namespace Engine