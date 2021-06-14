#include "Boss.h"
#include <Systems/NodeSystem.h>
#include <GameManager.h>

using namespace Engine;
using namespace Engine::ECS;

auto Boss::GetNode() -> std::shared_ptr< Engine::Node > {
  const auto& node_tr =
      Engine::ECS::EntityManager::GetComponent< Engine::Transform >(_currentNode->GetEntity());
  const auto delta      = node_tr->WorldPosition() - _transform->Position();
  const auto magnitude2 = glm::dot(delta, delta);
  auto min_mag          = 4.0f;
  if (magnitude2 <= (min_mag * min_mag)) {
    // if (_currentNode->NextIndex() == 3)
    // GameManager::ShowLevelSumUp(10.0f, true);
    _currentNode = _nodeSystem->GetNode(_currentNode->NextIndex(), Engine::NodeTag::Boss);
    _nodeTransform =
        Engine::ECS::EntityManager::GetComponent< Engine::Transform >(_currentNode->GetEntity());
  }
  return _currentNode;
}

Boss::Boss(std::shared_ptr< PlayerRect > player)
    : _player(player){}


auto Boss::OnCreate() -> void {
  _transform  = Entity()->GetComponent< Engine::Transform >();
  _nodeSystem = ECS::EntityManager::GetInstance().GetSystem< NodeSystem >();
  _currentNode    = _nodeSystem->GetNode(1, NodeTag::Boss);
  _nodeTransform  = EntityManager::GetComponent< Engine::Transform >(_currentNode->GetEntity());
  _playerSettings = GameManager::GetPlayerSettings();
}

auto Boss::Update(float deltaTime) -> void {

  _canMove = _player->CurrentNodeIndex() > 89;

  if (_canMove) {
    SeekTarget(deltaTime);
    HandleMove(deltaTime);
  }
}

auto Boss::OnKeyPressed(Engine::Key key) -> void {
}

auto Boss::CanMove() -> bool {
  return _canMove;
}

auto Boss::CanMove(bool value) -> void {
  _canMove = value;
}

auto Boss::Hit() -> void
{
  _hits++;
  //GameManager::GetSoundEngine()->play2D("./Assets/sounds/placeholderBeep.wav");
  if (_hits >= 3)
  {
    Engine::ECS::EntityManager::GetInstance().RemoveEntity(Entity());
    GameManager::Win();
  }
}

auto Boss::SeekTarget(float deltaTime) -> void {
  auto forward_speed = _playerSettings->ForwardSpeed();
  auto seek_speed    = _playerSettings->SeekSpeed();
  auto node          = GetNode();
  auto desired_velocity =
      glm::normalize(_nodeTransform->WorldPosition() - _transform->Position()) * forward_speed;
  auto velocity_delta = desired_velocity - _moveVelocity;
  auto new_velocity   = _moveVelocity + velocity_delta * seek_speed * deltaTime;
  _moveVelocity       = new_velocity;

  //_transform->Forward(glm::normalize(new_velocity));

  auto tmp = glm::lookAt(_transform->Position(), _transform->Position() + new_velocity,
                         {0.0f, 1.0f, 0.0f});
  tmp[3]   = glm::vec4(0.0f);
  tmp      = glm::transpose(tmp);
  auto rot = glm::quat_cast(tmp);
  _transform->Rotation(rot);
}

auto Boss::HandleMove(float deltaTime) -> void {
  _transform->Position(_transform->Position()
                       + glm::normalize(_moveVelocity) * _playerSettings->BossForwardSpeed()
                             * deltaTime);
}