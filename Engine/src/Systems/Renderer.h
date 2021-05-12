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

namespace Engine::GL {
  class Cubemap;
}

namespace Engine::Systems {
  class Renderer : public ECS::System {
  public:
    Renderer();
    ~Renderer() override = default;
    void Update(float deltaTime) override;
    auto AddEntity(ECS::EntityID id) -> void override;
    auto OnWindowResize(glm::vec2 windowSize) -> void;

  private:
    std::vector< ECS::EntityID > _visibleEntities;
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
    std::shared_ptr< LightSystem > _lightSystem;
    std::shared_ptr< Engine::Renderer::Material > _debugMaterial;

    std::shared_ptr< GL::Cubemap > cubemap;

    uint32_t _transformUniformSlot{0u};
    GL::TransformUniformData _transformUniformData;
    GL::TransformUniformBuffer _transformUniformBuffer;

    float _frustum[6][4];
    std::shared_ptr< GL::RenderTarget > _depthTarget;

    uint32_t _shadowUniformSlot{6u};
    GL::ShadowUniformBuffer _shadowUniformBuffer;
    GL::ShadowUniformData _shadowUniformData;
    std::shared_ptr< GL::Shader > _shadowMapShader;
    glm::vec2 _shadowMapSize;
    glm::mat4 _shadowProjection;
    std::shared_ptr< GL::RenderTarget > _shadowTarget;
    std::shared_ptr< GL::TextureAttachment > _shadowTexture;

    auto CalculateFrustrum(glm::mat4 clip) -> void;
    auto SphereInFrustum(glm::vec3 center, float radius) -> bool;
    auto PostProcessing() -> void;
    auto SortByMaterial() -> void;
    auto SortByDistance(std::shared_ptr< Camera > cam) -> void;

#if defined(_DEBUG)
    auto DrawNodes() -> void;
#endif
  };

}  // namespace Engine::Systems
