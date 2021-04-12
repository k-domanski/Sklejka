#pragma once
#include "ECS/Component.h"
#include "Renderer/Material.h"
#include "Renderer/Mesh.h"

namespace Engine::Components {
  class MeshRenderer : public ECS::Component {
  public:
    //MeshRenderer() = default;
    MeshRenderer(): Component(), _dirty(true) {
    }
    MeshRenderer(std::shared_ptr< Renderer::Mesh > mesh,
                 std::shared_ptr< Renderer::Material > material)
        : Component(), _mesh(mesh), _material(material), _dirty(true) {
    }
    ~MeshRenderer() override = default;

    auto GetMesh() -> std::shared_ptr< Renderer::Mesh >;
    auto SetMesh(std::shared_ptr< Renderer::Mesh > mesh) -> void;
    auto GetMaterial() -> std::shared_ptr< Renderer::Material >;
    auto SetMaterial(std::shared_ptr< Renderer::Material > material) -> void;
    auto IsDirty() -> bool;
    auto SetDirty(bool dirty) -> void;

  private:
    std::shared_ptr< Renderer::Mesh > _mesh;
    std::shared_ptr< Renderer::Material > _material;
    bool _dirty;
  };
}  // namespace Engine::Components