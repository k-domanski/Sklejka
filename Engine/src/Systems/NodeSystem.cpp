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

  auto NodeSystem::AddEntity(ECS::EntityID id) -> void {
    ECS::System::AddEntity(id);
    std::sort(_entities.begin(), _entities.end(), [](auto lhs, auto rhs) {
      auto ln = ECS::EntityManager::GetComponent< Node >(lhs);
      auto rn = ECS::EntityManager::GetComponent< Node >(rhs);
      return ln->Index() < rn->Index();
    });
  }

  auto NodeSystem::GetNode(int index) -> std::shared_ptr< Node > {
    for (auto id : _entities) {
      auto& node = ECS::EntityManager::GetComponent< Engine::Node >(id);
      if (node->Index() == index) {
        return node;
      }
    }
    return nullptr;
  }

}  // namespace Engine
