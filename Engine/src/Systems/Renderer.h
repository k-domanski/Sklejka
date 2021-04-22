#pragma once
#include "Components/MeshRenderer.h"
#include "Components/Transform.h"
#include "ECS/System.h"
#include "Renderer/Material.h"
#include "Renderer/Mesh.h"
#include <GL/RenderTarget.h>
#include <GL/Buffer.h>
#include <Systems/CameraSystem.h>

namespace Engine::Systems {
  class Renderer : public ECS::System {
  public:
    Renderer();
    ~Renderer() override = default;
    void Update(float deltaTime) override;
    auto AddEntity(ECS::EntityID id) -> void override;
    auto OnWindowResize(glm::vec2 windowSize) -> void;
    std::shared_ptr< Engine::GL::TextureAttachment > GetTextureID()  { return _screenTexture; }

  private:
    std::map< std::shared_ptr< Engine::Renderer::Material >, std::vector< ECS::EntityID > >
        _sortedEntities;
    std::set< ECS::EntityID > _entitiesToSort;
    std::shared_ptr< Engine::GL::RenderTarget > _renderTarget;
    std::shared_ptr< Engine::GL::TextureAttachment > _screenTexture;
    std::shared_ptr< Engine::Renderer::Mesh > _quad;
    std::shared_ptr< Engine::GL::Shader > _quadShader;

    std::shared_ptr< CameraSystem > _cameraSystem;

    uint32_t _transformUniformSlot{0u};
    GL::TransformUniformData _transformUniformData;
    GL::TransformUniformBuffer _transformUniformBuffer;

    auto SortByMaterial() -> void;
  };

}  // namespace Engine::Systems
