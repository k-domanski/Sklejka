#pragma once
#include "ECS/Component.h"
#include "Renderer/Material.h"
//#include "Renderer/Mesh.h"
#include "Renderer/Model.h"

namespace Engine::Components {
  class MeshRenderer : public ECS::Component {
  public:
    //MeshRenderer() = default;
    MeshRenderer(): Component("Mesh Renderer"), _dirty(true) {
    }
    MeshRenderer(std::shared_ptr< Renderer::Model > model,
                 std::shared_ptr< Renderer::Material > material)
        : Component("Mesh Renderer"), _model(model), _material(material), _dirty(true) {
    }
    ~MeshRenderer() override = default;

    auto GetModel() -> std::shared_ptr< Renderer::Model >;
    auto SetModel(std::shared_ptr< Renderer::Model > mesh) -> void;
    auto GetMaterial() -> std::shared_ptr< Renderer::Material >;
    auto SetMaterial(std::shared_ptr< Renderer::Material > material) -> void;
    auto IsDirty() -> bool;
    auto SetDirty(bool dirty) -> void;
    std::string SaveToJson(std::string filePath) override;
    auto LoadFromJson(std::string filePath) -> void override;

  private:
    std::shared_ptr< Renderer::Model > _model;
    std::shared_ptr< Renderer::Material > _material;
    bool _dirty;
  };
}  // namespace Engine::Components