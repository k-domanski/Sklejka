#include "pch.h"
#include "SceneGraph.h"
#include <Components/Transform.h>

namespace Engine::Systems {
  SceneGraph::SceneGraph(): ECS::System() {
    AddSignature< Transform >();
  }
  auto SceneGraph::Update() -> void {
    using ECS::EntityManager;
    // Check if any transform is dirty and update it's branch
    for (auto& kv : _parentChildMap) {
      if (kv.first == _rootID)
        continue;
      auto& transform = EntityManager::GetInstance().GetComponent< Transform >(kv.first);
      if (transform->flags.GetFlag(TransformFlags::Dirty)) {
        auto& parent_tr =
            EntityManager::GetInstance().GetComponent< Transform >(_childParentMap[kv.first]);
        RecursiveUpdateParentAndChildren(transform, parent_tr);
      }
    }
  }
  auto SceneGraph::SetParent(ECS::EntityID child, ECS::EntityID parent) -> void {
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
    _parentChildMap[parent].push_back(child);
    _childParentMap[child] = parent;
  }
  auto SceneGraph::AddChild(ECS::EntityID parent, ECS::EntityID child) -> void {
    SetParent(child, parent);
  }
  auto SceneGraph::AddEntity(ECS::EntityID id) -> void {
    SetParent(id, _rootID);
    //ECS::System::AddEntity(id);
  }
  auto SceneGraph::AddEntity(ECS::EntityID id, ECS::EntityID parent) -> void {
    ECS::System::AddEntity(id);
    SetParent(id, parent);
  }
  auto SceneGraph::RecursiveUpdateParentAndChildren(std::shared_ptr< Transform >& transform,
                                                    std::shared_ptr< Transform > parent) -> void {
    if (parent == nullptr) {
      transform->_modelMatrix = transform->GetLocalMatrix();
    } else {
      transform->_modelMatrix = parent->_modelMatrix * transform->GetLocalMatrix();
    }
    transform->flags.ClearFlag(TransformFlags::Dirty);
    if (_parentChildMap.count(transform->GetEntityID()) == 0)
      return;
    for (auto& childID : _parentChildMap[transform->GetEntityID()]) {
      auto& child_tr = ECS::EntityManager::GetInstance().GetComponent< Transform >(childID);
      RecursiveUpdateParentAndChildren(child_tr, transform);
    }
  }
}  // namespace Engine::Systems
