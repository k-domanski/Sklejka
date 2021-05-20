#include "PlayerRect.h"
#include <Systems/NodeSystem.h>
#include <GameManager.h>

using namespace Engine;
using namespace Engine::ECS;

PlayerRect::PlayerRect(const std::shared_ptr< PlayerController >& player_controller)
    : _playerController(player_controller) {
}

auto PlayerRect::OnCreate() -> void {
  _transform      = Entity()->GetComponent< Engine::Transform >();
  _nodeSystem     = ECS::EntityManager::GetInstance().GetSystem< NodeSystem >();
  _currentNode    = _nodeSystem->GetNode(0);
  _nodeTransform  = EntityManager::GetComponent< Engine::Transform >(_currentNode->GetEntityID());
  _playerSettings = GameManager::GetPlayerSettings();
}

auto PlayerRect::Update(float deltaTime) -> void {
  float vertical_move, horizontal_move, roll;

  HandleInput(vertical_move, horizontal_move, roll);
  SeekTarget(deltaTime);
  HandleMove(vertical_move, horizontal_move, deltaTime);
  HandleRotation(roll, deltaTime);
}

// auto PlayerRect::Size() const noexcept -> glm::vec2 {
//  return _size;
//}
//
// auto PlayerRect::Size(const glm::vec2& size) noexcept -> glm::vec2 {
//  return _size = size;
//}

auto PlayerRect::HandleInput(float& vertical, float& horizontal, float& roll) -> void {
  vertical   = ((float)Input::IsKeyPressed(Key::W)) - ((float)Input::IsKeyPressed(Key::S));
  horizontal = ((float)Input::IsKeyPressed(Key::D)) - ((float)Input::IsKeyPressed(Key::A));
  roll       = ((float)Input::IsKeyPressed(Key::E)) - ((float)Input::IsKeyPressed(Key::Q));

  vertical += Input::GetGamepadAxis(GamepadAxes::VERTICAL_LEFT);
  horizontal += Input::GetGamepadAxis(GamepadAxes::HORIZONTAL_LEFT);
  roll += (Input::GetGamepadAxis(GamepadAxes::TRIGGER_RIGHT)
           - Input::GetGamepadAxis(GamepadAxes::TRIGGER_LEFT));

  vertical   = glm::clamp(vertical, -1.0f, 1.0f);
  horizontal = glm::clamp(horizontal, -1.0f, 1.0f);
  roll       = glm::clamp(roll, -1.0f, 1.0f);
}

auto PlayerRect::SeekTarget(float deltaTime) -> void {
  auto forward_speed = _playerSettings->ForwardSpeed();
  auto seek_speed    = _playerSettings->SeekSpeed();
  auto node          = GetNode();
  auto desired_velocity =
      glm::normalize(_nodeTransform->Position() - _transform->Position()) * forward_speed;
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

auto PlayerRect::HandleMove(float vertical, float horizontal, float deltaTime) -> void {
  auto move_delta = glm::vec3{horizontal, -vertical, 0.0f};
  // move_delta += -vertical * _transform->Up();
  // move_delta += horizontal * _transform->Right();

  // move_delta     = (glm::normalize(_moveVelocity) + move_delta) * _speed * deltaTime;
  move_delta     = move_delta * _playerSettings->ControlSpeed() * deltaTime;
  auto pos       = _playerController->Transform()->Position();
  auto new_pos   = pos + move_delta;
  auto half_size = glm::vec3(_playerSettings->RectSize(), 0.0f) * 0.5f;

  new_pos = glm::clamp(new_pos, -half_size, half_size);
  _playerController->Transform()->Position(new_pos);

  _transform->Position(_transform->Position()
                       + glm::normalize(_moveVelocity) * _playerSettings->ForwardSpeed()
                             * deltaTime);
}

auto PlayerRect::HandleRotation(float roll, float deltaTime) -> void {
  _playerController->Transform()->Rotate(roll * deltaTime, {0.0f, 0.0f, -1.0f});
}

auto PlayerRect::GetNode() -> std::shared_ptr< Engine::Node > {
  const auto& node_tr =
      EntityManager::GetComponent< Engine::Transform >(_currentNode->GetEntityID());
  const auto delta      = node_tr->Position() - _transform->Position();
  const auto magnitude2 = glm::dot(delta, delta);
  auto min_mag          = _playerSettings->MinNodeDistance();
  if (magnitude2 <= (min_mag * min_mag)) {
    _currentNode   = _nodeSystem->GetNode(_currentNode->NextIndex());
    _nodeTransform = EntityManager::GetComponent< Engine::Transform >(_currentNode->GetEntityID());
  }
  return _currentNode;
}
