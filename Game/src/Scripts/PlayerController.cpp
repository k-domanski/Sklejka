#include "PlayerController.h"
#include <Systems/NodeSystem.h>

using namespace Engine;
using namespace Engine::ECS;
PlayerController::PlayerController(const std::shared_ptr< Engine::Transform >& player_transform)
    : _playerTransform(player_transform) {
}

auto PlayerController::OnCreate() -> void {
  _nodeSystem    = ECS::EntityManager::GetInstance().GetSystem< NodeSystem >();
  _currentNode   = _nodeSystem->GetNode(0);
  _nodeTransform = EntityManager::GetComponent< Transform >(_currentNode->GetEntityID());
}

auto PlayerController::Update(float deltaTime) -> void {
  // auto new_pos = _playerTransform->Position() + _playerTransform->Forward() * _speed * deltaTime;
  /* Replace it with Transform Right/Up/Forward once model gets the correct pivot */

  /* Seek target */
  SeekTarget(deltaTime);

  auto vertical_move =
      ((float)Engine::Input::IsKeyPressed(Key::W)) - ((float)Engine::Input::IsKeyPressed(Key::S));
  auto horizontal_move =
      ((float)Engine::Input::IsKeyPressed(Key::D)) - ((float)Engine::Input::IsKeyPressed(Key::A));
  auto roll =
      ((float)Engine::Input::IsKeyPressed(Key::E)) - ((float)Engine::Input::IsKeyPressed(Key::Q));

  auto move_delta = glm::vec3{0.0f};
  move_delta += vertical_move * -_playerTransform->Up();
  move_delta += horizontal_move * -_playerTransform->Right();

  //_playerTransform->Position(_playerTransform->Position() + move_delta * _speed * deltaTime);
  _playerTransform->Position(_playerTransform->Position()
                             + (glm::normalize(_moveVelocity) + move_delta) * _speed * deltaTime);
  _playerTransform->Rotate(roll * deltaTime, _playerTransform->Forward());
}

auto PlayerController::SeekTarget(float deltaTime) -> void {
  auto node = GetNode();
  auto desired_velocity =
      glm::normalize(_nodeTransform->Position() - _playerTransform->Position()) * _speed;
  auto velocity_delta = desired_velocity - _moveVelocity;
  auto new_velocity   = _moveVelocity + velocity_delta * _seekSpeed * deltaTime;
  _moveVelocity       = new_velocity;
  _playerTransform->Forward(glm::normalize(new_velocity));
}

auto PlayerController::GetNode() -> std::shared_ptr< Engine::Node > {
  const auto& node_tr   = EntityManager::GetComponent< Transform >(_currentNode->GetEntityID());
  const auto delta      = node_tr->Position() - _playerTransform->Position();
  const auto magnitude2 = glm::dot(delta, delta);
  if (magnitude2 <= (_minNodeDistance * _minNodeDistance)) {
    _currentNode   = _nodeSystem->GetNode(_currentNode->NextIndex());
    _nodeTransform = EntityManager::GetComponent< Transform >(_currentNode->GetEntityID());
  }
  return _currentNode;
}
