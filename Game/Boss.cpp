#include "Boss.h"

auto Boss::GetNode() -> std::shared_ptr<Engine::Node>
{
  const auto& node_tr   = Engine::ECS::EntityManager::GetComponent< Engine::Transform >(_currentNode->GetEntity());
  const auto delta      = node_tr->WorldPosition() - _transform->Position();
  const auto magnitude2 = glm::dot(delta, delta);
  auto min_mag          = 4.0f;
  if (magnitude2 <= (min_mag * min_mag)) {
    // if (_currentNode->NextIndex() == 3)
    // GameManager::ShowLevelSumUp(10.0f, true);
    _currentNode   = _nodeSystem->GetNode(_currentNode->NextIndex(), Engine::NodeTag::Boss);
    _nodeTransform = Engine::ECS::EntityManager::GetComponent< Engine::Transform >(_currentNode->GetEntity());
  }
  return _currentNode;
}

Boss::Boss() {}

using namespace Engine;
using namespace Engine::ECS;

auto Boss::OnCreate() -> void {
  _transform  = Entity()->GetComponent< Engine::Transform >();
  _nodeSystem = ECS::EntityManager::GetInstance().GetSystem< NodeSystem >();
}

auto Boss::Update(float deltaTime) -> void{
  SeekTarget(deltaTime);
  HandleMove(deltaTime);
}

auto Boss::OnKeyPressed(Engine::Key key) -> void  { }
