#pragma once
#include "Components/MeshRenderer.h"
#include "Components/Transform.h"
#include "ECS/System.h"
#include "Renderer/Material.h"
#include "Renderer/Mesh.h"

namespace Engine::Systems {
  class Renderer : public Engine::ECS::System {
  public:
    Renderer(): System() {
      AddSignature< Components::MeshRenderer >();
      AddSignature< Transform >();
    }
    ~Renderer() override = default;
    void Update(float deltaTime) override;
    auto AddEntity(ECS::EntityID id) -> void override;

  private:
    std::map< std::shared_ptr< Engine::Renderer::Material >, std::vector< ECS::EntityID > >
        _sortedEntities;
    std::set< ECS::EntityID > _entitiesToSort;

    auto SortByMaterial() -> void;
  };

}  // namespace Engine::Systems
