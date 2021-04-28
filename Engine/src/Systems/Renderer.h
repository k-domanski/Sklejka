#pragma once
#include "Components/MeshRenderer.h"
#include "Components/Transform.h"
#include "ECS/System.h"
#include "Renderer/Material.h"
#include "Renderer/Mesh.h"
#include <GL/RenderTarget.h>
#include <GL/Buffer.h>
#include <Systems/CameraSystem.h>
#include <Renderer/PingPongBuffer.h>

namespace Engine::Systems {
  class Renderer : public ECS::System {
  public:
    Renderer();
    ~Renderer() override = default;
    void Update(float deltaTime) override;
    auto AddEntity(ECS::EntityID id) -> void override;
    auto OnWindowResize(glm::vec2 windowSize) -> void;

  private:
    std::map< std::shared_ptr< Engine::Renderer::Material >, std::vector< ECS::EntityID > >
        _sortedEntities;
    std::set< ECS::EntityID > _entitiesToSort;
    std::shared_ptr< Engine::Renderer::PingPongBuffer > _pingPongBuffer;
    std::shared_ptr< Engine::Renderer::Mesh > _quad;
    std::shared_ptr< Engine::GL::Shader > _blurShader;
    std::shared_ptr< Engine::GL::Shader > _fishEyeShader;
    std::shared_ptr< Engine::GL::Shader > _finalPassShader;

    std::shared_ptr< Engine::Renderer::Mesh > _boxCollider;
    std::shared_ptr< Engine::GL::Shader > _boxColliderShader;

    std::shared_ptr< Engine::Renderer::Mesh > _sphereCollider;
    std::shared_ptr< Engine::GL::Shader > _sphereColliderShader;

    std::shared_ptr< CameraSystem > _cameraSystem;
    std::shared_ptr< Engine::Renderer::Material > _debugMaterial;

    uint32_t _transformUniformSlot{0u};
    GL::TransformUniformData _transformUniformData;
    GL::TransformUniformBuffer _transformUniformBuffer;

    float _frustum[6][4];

    auto CalculateFrustrum(glm::mat4 proj, glm::mat4 modelView) -> void;
    auto SphereInFrustum(glm::vec3 center, float radius) -> bool;
    auto PostProcessing() -> void;
    auto SortByMaterial() -> void;
  };

}  // namespace Engine::Systems
