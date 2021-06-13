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
#include <Systems/LightSystem.h>
#include <Systems/ParticleSystem.h>

#include "../../../Game/src/Scripts/ShadowTarget.h"

namespace Engine::GL {
  class Cubemap;
}

namespace Engine::Systems {
  class Renderer : public ECS::System {
  public:
    Renderer();
    ~Renderer() override = default;
    void Update(float deltaTime) override;
    auto OnWindowResize(glm::vec2 windowSize) -> void;
    // auto SetShadowChecker(std::shared_ptr< ShadowTarget > target) -> void;
    auto ObjectInShadow(const std::shared_ptr< ECS::Entity >& entity) -> GLint;
    auto AddEntity(const std::shared_ptr< ECS::Entity >& entity) -> void override;
    auto SetShouldSort(bool value) -> void;

  private:
    /* Switches */
    bool _cullingEnabled = false;
    /* -=-=-=-=- */

    /* Data */
    std::vector< std::shared_ptr< ECS::Entity > > _visibleEntities;
    GL::TransformUniformData _transformUniformData;
    GL::TransformUniformBuffer _transformUniformBuffer;
    bool _shouldSort = true;
    /* -=-=- */

    /* Systems */
    std::shared_ptr< CameraSystem > _cameraSystem;
    std::shared_ptr< LightSystem > _lightSystem;
    std::shared_ptr< ParticleSystem > _particleSystem;
    /* -=-=-=- */

    /* Animation */
    GL::JointsUniformBuffer m_JointBuffer;
    /* -=-=-=-=- */

    /* Skybox */
    uint32_t _skyboxSlot{7u};
    std::shared_ptr< Engine::Renderer::Mesh > _cube;
    std::shared_ptr< GL::Cubemap > _cubemap;
    std::shared_ptr< GL::Shader > _skyboxShader;
    /* -=-=-=- */

    /* Shadow Mapping */
    uint32_t _shadowMapSlot{6u};
    glm::vec2 _shadowMapSize;
    glm::mat4 _shadowProjection;
    GL::ShadowUniformBuffer _shadowUniformBuffer;
    GL::ShadowUniformData _shadowUniformData;
    std::shared_ptr< GL::Shader > _shadowMapShader;
    std::shared_ptr< GL::Shader > _shadowMapAnimShader;
    std::shared_ptr< GL::RenderTarget > _shadowTarget;
    std::shared_ptr< GL::TextureAttachment > _shadowTexture;
    /* -=-=-=-=-=-=-=- */

    /* Occlusion Culling (?) */
    float _frustum[6][4];
    std::shared_ptr< GL::RenderTarget > _depthTarget;
    /* -=-=-=-=-=-=-=-=-=-=- */

    /* Post Processing */
    std::shared_ptr< Engine::Renderer::PingPongBuffer > _pingPongBuffer;
    std::shared_ptr< Engine::Renderer::Mesh > _quad;
    std::shared_ptr< Engine::GL::Shader > _blurShader;
    std::shared_ptr< Engine::GL::Shader > _fishEyeShader;
    std::shared_ptr< Engine::GL::Shader > _finalPassShader;
    /* -=-=-=-=-=-=-=- */

    /* DEBUG */
    std::shared_ptr< Engine::Renderer::Material > _debugMaterial;

    std::shared_ptr< Engine::Renderer::Mesh > _boxCollider;
    std::shared_ptr< Engine::Renderer::Mesh > _sphereCollider;

    std::shared_ptr< Engine::GL::Shader > _boxColliderShader;
    std::shared_ptr< Engine::GL::Shader > _sphereColliderShader;
    /* -=-=- */

    auto CalculateFrustrum(glm::mat4 clip) -> void;
    auto SphereInFrustum(glm::vec3 center, float radius) -> bool;
    auto PostProcessing() -> void;
    auto DrawSkybox() -> void;
    auto DrawParticles() -> void;
    auto SortByMaterial() -> void;
    auto SortByDistance(std::shared_ptr< Camera > cam) -> void;

#if defined(_DEBUG)
    auto DrawColliders() -> void;
    auto DrawNodes() -> void;
#endif
  };

}  // namespace Engine::Systems
