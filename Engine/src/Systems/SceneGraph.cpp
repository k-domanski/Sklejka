#include "pch.h"
#include "SceneGraph.h"
#include <Components/Transform.h>

namespace Engine::Systems {
  using ECS::EntityManager;
  SceneGraph::SceneGraph() {
    AddSignature< Transform >();
    _rootEntity = EntityManager::GetInstance().CreateEntity(_rootID);
    _rootEntity->Name("Scene");
    _rootEntity->AddComponent< Transform >();
    AddEntity(_rootEntity);
  }
  auto SceneGraph::Update(float deltaTime) -> void {
// Check if any transform is dirty and update it's branch
#if defined(_DEBUG)
    static int force_counter = 0;
    if (deltaTime < 0) {
      force_counter++;
      LOG_DEBUG("Forced graph updates {}", force_counter);
    } else {
      force_counter = 0;
    }
#endif
    for (auto& entity : _entities) {
      auto& transform = EntityManager::GetComponent< Transform >(entity);
      if (transform->flags.Get(TransformFlag::Dirty)) {
        auto& parent_tr =
            EntityManager::GetComponent< Transform >(_childParentMap[entity->GetID()]);
        RecursiveUpdateParentAndChildren(transform, parent_tr);
        transform->flags.Clear(TransformFlag::Dirty);
      }
    }
  }
  auto SceneGraph::SetParent(const std::shared_ptr< ECS::Entity >& child,
                             const std::shared_ptr< ECS::Entity >& parent) -> void {
    // Remove from current parent
    for (auto& kv : _parentChildMap) {
      if (kv.second.size() == 0)
        continue;
      auto it = std::find(kv.second.begin(), kv.second.end(), child);
      if (it != kv.second.end()) {
        kv.second.erase(it);
        break;
      }
    }
    // Assign to new parent
    auto c_id = child != nullptr ? child->GetID() : 0;
    auto p_id = parent != nullptr ? parent->GetID() : 0;
    if (c_id != _rootID) {
      _parentChildMap[p_id].push_back(child);
      _childParentMap[c_id] = parent;
    }
  }
  auto SceneGraph::AddChild(const std::shared_ptr< ECS::Entity >& parent,
                            const std::shared_ptr< ECS::Entity >& child) -> void {
    SetParent(child, parent);
  }
  auto SceneGraph::DetachFromGraph(const std::shared_ptr< ECS::Entity >& entity) -> void {
    // Find parent
    if (_childParentMap.count(entity->GetID()) == 0) {  // entity not in graph
      return;
    }
    auto parent       = _childParentMap[entity->GetID()];
    auto parent_id    = parent != nullptr ? parent->GetID() : 0;
    auto& parent_list = _parentChildMap[parent_id];
    auto it           = std::find_if(parent_list.begin(), parent_list.end(),
                           [&entity](auto& item) { return entity->GetID() == item->GetID(); });
    assert(("Child had registered parent, but parent did not have this child registered.",
            it != parent_list.end()));
    parent_list.erase(it);
    _childParentMap.erase(entity->GetID());
  }

  auto SceneGraph::ClearFlags() -> void {
    for (auto& entity : _entities) {
      auto& tr = entity->GetComponent< Transform >();
      tr->flags.Clear(TransformFlag::NewData);
    }
  }

  auto SceneGraph::GetRootID() -> ECS::EntityID {
    return _rootID;
  }

  auto SceneGraph::GetRootEntity() -> std::shared_ptr< ECS::Entity > {
    return _rootEntity;
  }

  auto SceneGraph::GetChildren(const std::shared_ptr< ECS::Entity >& entity)
      -> std::vector< std::shared_ptr< ECS::Entity > >& {
    auto id = entity != nullptr ? entity->GetID() : 0;
    return _parentChildMap[id];
  }

  auto SceneGraph::GetParent(const std::shared_ptr< ECS::Entity >& entity)
      -> std::shared_ptr< ECS::Entity > {
    return _childParentMap[entity->GetID()];
  }

  auto SceneGraph::RemoveEntity(const std::shared_ptr< ECS::Entity >& entity) -> void {
    if (entity == nullptr) {
      return;
    }
    const auto& parent = _childParentMap[entity->GetID()];
    auto parent_id     = parent != nullptr ? parent->GetID() : 0;
    auto& list         = _parentChildMap[parent_id];
    auto it            = std::find(list.begin(), list.end(), entity);
    if (it != list.end()) {
      list.erase(it);
    }
    _childParentMap.erase(entity->GetID());
  }

  auto SceneGraph::RecursiveUpdateParentAndChildren(std::shared_ptr< Transform >& transform,
                                                    std::shared_ptr< Transform >& parent) -> void {
    if (parent == nullptr) {
      transform->_modelMatrix = transform->GetLocalMatrix();
    } else {
      transform->_modelMatrix = parent->_modelMatrix * transform->GetLocalMatrix();
    }
    transform->flags.Clear(TransformFlag::Dirty);
    transform->flags.Set(TransformFlag::NewData);
    if (_parentChildMap.count(transform->GetEntityID()) == 0)
      return;
    for (auto& child : _parentChildMap[transform->GetEntityID()]) {
      auto& child_tr = ECS::EntityManager::GetComponent< Transform >(child);
      RecursiveUpdateParentAndChildren(child_tr, transform);
    }
  }
}  // namespace Engine::Systems
