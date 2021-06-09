#include "PlayerRect.h"
#include <Systems/NodeSystem.h>
#include <GameManager.h>

using namespace Engine;
using namespace Engine::ECS;

PlayerRect::PlayerRect(const std::shared_ptr<PlayerController>& player_controller,
                       const std::shared_ptr< Engine::Transform >& player_model)
    : _playerController(player_controller), _modelTransform(player_model) {
}

auto PlayerRect::OnCreate() -> void {
  _transform      = Entity()->GetComponent< Engine::Transform >();
  _nodeSystem     = ECS::EntityManager::GetInstance().GetSystem< NodeSystem >();
  _currentNode    = _nodeSystem->GetNode(1);
  _nodeTransform  = EntityManager::GetComponent< Engine::Transform >(_currentNode->GetEntity());
  _playerSettings = GameManager::GetPlayerSettings();
  _gameSettings   = GameManager::GetGameSettings();
}

auto PlayerRect::Update(float deltaTime) -> void {
  float vertical_move, horizontal_move, roll;

  HandleInput(vertical_move, horizontal_move, roll);
  SeekTarget(deltaTime);
  HandleMove(vertical_move, horizontal_move, deltaTime);
  HandleRotation(roll, deltaTime);
}

auto PlayerRect::OnKeyPressed(Engine::Key key) -> void {
}

auto PlayerRect::CanMove() -> bool {
  return _canMove;
}

auto PlayerRect::CanMove(bool value) -> void {
  _canMove = value;
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

  if (Input::IsKeyPressed(Key::X))
    GameManager::GetSoundEngine()->play2D("./Assets/sounds/placeholderBeep.wav", false);
}

auto PlayerRect::SeekTarget(float deltaTime) -> void {
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

auto PlayerRect::HandleMove(float vertical, float horizontal, float deltaTime) -> void {
  auto move_delta = glm::vec3{horizontal, -vertical, 0.0f};
  // move_delta += -vertical * _transform->Up();
  // move_delta += horizontal * _transform->Right();

  // move_delta     = (glm::normalize(_moveVelocity) + move_delta) * _speed * deltaTime;
  if (_canMove) {
    move_delta     = move_delta * _playerSettings->ControlSpeed() * deltaTime;
    auto pos       = _playerController->Transform()->Position();
    auto new_pos   = pos + move_delta;
    auto half_size = glm::vec3(_playerSettings->RectSize(), 0.0f) * 0.5f;

    auto new_pos_clamped = glm::clamp(new_pos, -half_size, half_size);
    _playerController->Transform()->Position(new_pos_clamped);


    auto rotation_delta = (move_delta / deltaTime) * _playerSettings->MoveRotationSpeed() * -1.f;
    //auto rotation = _playerController->Transform()->Rotation();

    if (new_pos.x < half_size.x && new_pos.x > -half_size.x)
      HandleModelRotation(rotation_delta.x, deltaTime, {0.f, 1.f, 0.f});
    if (new_pos.y < half_size.y && new_pos.y > -half_size.y)
      HandleModelRotation(rotation_delta.y, deltaTime, {1.f, 0.f, 0.f});

    //if (rotation_delta.x == 0.f && rotation_delta.y == 0.f)
    //  LerpResetRotation();
  }

  _transform->Position(_transform->Position()
                       + glm::normalize(_moveVelocity) * _playerSettings->ForwardSpeed() * deltaTime
                             * _gameSettings->PlayerTimeScale());
}

auto PlayerRect::HandleRotation(float roll, float deltaTime, glm::vec3 axis) -> void
{
  if (_canMove) {
    _playerController->Transform()->Rotate(roll * deltaTime, axis);
  }
}

auto PlayerRect::HandleModelRotation(float roll, float deltaTime, glm::vec3 axis) -> void
{
  if (_canMove) {
    if (roll != 0.f) {
      if ((abs(_modelTransform->Rotation().x) < 0.22f && axis.x != 0.f)
          || (abs(_modelTransform->Rotation().y) < 0.22f && axis.y != 0.f)) {
        _modelTransform->Rotate(roll * deltaTime, axis);
      }
    } else{
      auto currentRot = _modelTransform->Rotation();
      auto desiredRot =
          glm::quat(1.f, axis.x == 0.f ? currentRot.x : 0,
                    axis.y == 0.f ? currentRot.y : 0, axis.z == 0.f ? currentRot.z : 0);

      _modelTransform->Rotation(glm::lerp(currentRot, desiredRot, 0.1f));
    }
  }
}

auto PlayerRect::LerpResetRotation() -> void
{
  auto currentRot = _modelTransform->Rotation();
  auto desiredRot = glm::quat(1.f, 0.f, 0.f, currentRot.z);

  LOG_DEBUG("x: " + std::to_string(glm::lerp(currentRot, desiredRot, 0.1f).x));
  LOG_DEBUG("y: " + std::to_string(glm::lerp(currentRot, desiredRot, 0.1f).y));
  LOG_DEBUG("w: " + std::to_string(glm::lerp(currentRot, desiredRot, 0.1f).w));

  _modelTransform->Rotation(glm::lerp(currentRot, desiredRot, 0.1f));
}

auto PlayerRect::GetNode() -> std::shared_ptr< Engine::Node > {
  const auto& node_tr = EntityManager::GetComponent< Engine::Transform >(_currentNode->GetEntity());
  const auto delta    = node_tr->WorldPosition() - _transform->Position();
  const auto magnitude2 = glm::dot(delta, delta);
  auto min_mag          = _playerSettings->MinNodeDistance();
  if (magnitude2 <= (min_mag * min_mag)) {
    //if (_currentNode->NextIndex() == 3)
      //GameManager::ShowLevelSumUp(10.0f, true);
    _currentNode   = _nodeSystem->GetNode(_currentNode->NextIndex());
    _nodeTransform = EntityManager::GetComponent< Engine::Transform >(_currentNode->GetEntity());
  }
  return _currentNode;
}
