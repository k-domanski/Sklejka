#include "pch.h"
#include "NodeSystem.h"
#include "Components/Transform.h"
#include "Components/Node.h"

namespace Engine {
  NodeSystem::NodeSystem() {
    AddSignature< Transform >();
    AddSignature< Node >();
  }

  void NodeSystem::Update(float deltaTime) {
  }

}  // namespace Engine
