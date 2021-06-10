#include "pch.h"
#include "NodeSystem.h"
#include "Components/Transform.h"
#include "Components/Node.h"
#include "ECS/EntityManager.h"

namespace Engine {
  NodeSystem::NodeSystem() {
    AddSignature< Transform >();
    AddSignature< Node >();
  }

  void NodeSystem::Update(float deltaTime) {
  }

  auto NodeSystem::AddEntity(const std::shared_ptr< ECS::Entity >& entity) -> void {
    ECS::System::AddEntity(entity);
    std::sort(_entities.begin(), _entities.end(), [](auto lhs, auto rhs) {
      auto ln = ECS::EntityManager::GetComponent< Node >(lhs);
      auto rn = ECS::EntityManager::GetComponent< Node >(rhs);
      return ln->Index() < rn->Index();
    });
  }

  auto NodeSystem::GetNode(int index, NodeTag tag) -> std::shared_ptr< Node > {
    for (auto id : _entities) {
      auto& node = ECS::EntityManager::GetComponent< Engine::Node >(id);
      if (node->Tag() == tag && node->Index() == index) {
        return node;
      }
    }
    return nullptr;
  }

}  // namespace Engine
