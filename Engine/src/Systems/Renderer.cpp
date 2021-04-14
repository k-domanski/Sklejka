#include "pch.h"
#include "Renderer.h"

#include "Components/MeshRenderer.h"
#include "Components/Transform.h"
#include "ECS/EntityManager.h"

void Engine::Systems::Renderer::Update() {
  SortByMaterial();
  for (auto [material, vec] : _sortedEntities) {
    if (material == nullptr)
      continue;
    material->Use();
    for (auto entityID : vec) {
      auto meshRenderer =
          ECS::EntityManager ::GetInstance().GetComponent< Components::MeshRenderer >(entityID);
      auto mesh      = meshRenderer->GetMesh();
      auto transform = ECS::EntityManager ::GetInstance().GetComponent< Transform >(entityID);
      mesh->Use();
      // TODO: change for world matrix
      material->SetTransform(transform->GetWorldMatrix());
      glDrawElements(mesh->GetPrimitive(), mesh->ElementCount(), GL_UNSIGNED_INT, NULL);
    }
  }
}

auto Engine::Systems::Renderer::AddEntity(ECS::EntityID id) -> void {
  System::AddEntity(id);
}

auto Engine::Systems::Renderer::SortByMaterial() -> void {
  std::set< ECS::EntityID > entitiesToSort;

  for (auto entityID : _entities) {
    auto meshRenderer =
        ECS::EntityManager::GetInstance().GetComponent< Components::MeshRenderer >(entityID);
    if (meshRenderer->IsDirty()) {
      entitiesToSort.insert(entityID);
      meshRenderer->SetDirty(false);
    }
  }

  for (auto entityID : entitiesToSort) {
    auto meshRenderer =
        ECS::EntityManager::GetInstance().GetComponent< Components::MeshRenderer >(entityID);
    auto material = meshRenderer->GetMaterial();
    if (material == nullptr)
      continue;
    for (auto [materialFRST, vec] : _sortedEntities) {
      auto it =
          std::find_if(vec.begin(), vec.end(), [entityID](auto id) { return id == entityID; });
      if (it != vec.end()) {
        vec.erase(it);
        break;
      }
    }
    if (_sortedEntities.count(material) == 0) {
      std::vector< ECS::EntityID > vec;
      vec.push_back(entityID);
      _sortedEntities[material] = vec;
    } else
      _sortedEntities[material].push_back(entityID);
  }
}