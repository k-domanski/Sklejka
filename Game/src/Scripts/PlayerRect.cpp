#include "PlayerRect.h"
#include <Systems/NodeSystem.h>
#include <GameManager.h>

using namespace Engine;
using namespace Engine::ECS;

PlayerRect::PlayerRect(const std::shared_ptr< PlayerController >& player_controller,
                       const std::shared_ptr< Engine::Transform >& player_model)
    : _playerController(player_controller), _modelTransform(player_model) {
}

auto PlayerRect::OnCreate() -> void {
  _transform      = Entity()->GetComponent< Engine::Transform >();
  _nodeSystem     = ECS::EntityManager::GetInstance().GetSystem< NodeSystem >();
  _currentNode    = _nodeSystem->GetNode(1, NodeTag::Player);
  _nodeTransform  = EntityManager::GetComponent< Engine::Transform >(_currentNode->GetEntity());
  _playerSettings = GameManager::GetPlayerSettings();
  _gameSettings   = GameManager::GetGameSettings();

  _qBase    = _modelTransform->Rotation();
  _vRad     = glm::radians(30.0f); /* Param this */
  _hRad     = glm::radians(30.0f);
  _lerpTime = 0.25f; /* Param this */

  _qIden  = glm::quat({0.0f, 0.0f, 0.0f});
  _qUp    = glm::quat({_vRad, 0.0f, 0.0f});
  _qDown  = glm::inverse(_qUp);
  _qLeft  = glm::quat({0.0f, _hRad, 0.0f});
  _qRight = glm::inverse(_qLeft);

  _vLerp.Set(_qIden, _qIden, 1.0f);
  _hLerp.Set(_qIden, _qIden, 1.0f);
}

auto PlayerRect::Update(float deltaTime) -> void {
  float vertical_move, horizontal_move, roll;
  if (!_enabled) {
    return;
  }

  HandleInput(vertical_move, horizontal_move, roll);
  SeekTarget(deltaTime);
  HandleMove(vertical_move, horizontal_move, deltaTime);
  HandleRotation(roll, deltaTime);
  ModelRotation(vertical_move, horizontal_move, deltaTime);
}

auto PlayerRect::OnKeyPressed(Engine::Key key) -> void {
}

auto PlayerRect::CanMove() -> bool {
  return _canMove;
}

auto PlayerRect::CanMove(bool value) -> void {
  _canMove = value;
}

auto PlayerRect::Enable(bool value) -> void {
  _enabled = value;
}

auto PlayerRect::CurrentNodeIndex() -> int {
  if (_currentNode != nullptr)
    return _currentNode->Index();
  else
    return -1;
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

  vertical -= Input::GetGamepadAxis(GamepadAxes::VERTICAL_LEFT);
  horizontal += Input::GetGamepadAxis(GamepadAxes::HORIZONTAL_LEFT);
  roll += (Input::GetGamepadAxis(GamepadAxes::TRIGGER_RIGHT)
           - Input::GetGamepadAxis(GamepadAxes::TRIGGER_LEFT));

  vertical   = glm::clamp(vertical, -1.0f, 1.0f);
  horizontal = glm::clamp(horizontal, -1.0f, 1.0f);
  roll       = glm::clamp(roll, -1.0f, 1.0f);

  /*if (Input::IsKeyPressed(Key::X))
    GameManager::GetSoundEngine()->play2D("./Assets/sounds/placeholderBeep.wav", false);*/
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
    // auto rotation = _playerController->Transform()->Rotation();

    /*if (new_pos.x < half_size.x && new_pos.x > -half_size.x)
      HandleModelRotation(rotation_delta.x, deltaTime, {0.f, 1.f, 0.f});
    else
      HandleModelRotation(0.f, deltaTime, {0.f, 1.f, 0.f});

    if (new_pos.y < half_size.y && new_pos.y > -half_size.y)
      HandleModelRotation(rotation_delta.y, deltaTime, {1.f, 0.f, 0.f});
    else
      HandleModelRotation(0.f, deltaTime, {1.f, 0.f, 0.f});*/
  }

  _transform->Position(_transform->Position()
                       + glm::normalize(_moveVelocity) * _playerSettings->ForwardSpeed() * deltaTime
                             * _gameSettings->PlayerTimeScale());
}

auto PlayerRect::HandleRotation(float roll, float deltaTime, glm::vec3 axis) -> void {
  if (_canMove) {
    _playerController->Transform()->Rotate(roll * _playerSettings->RollSpeed() * deltaTime, axis);
  }
}

auto PlayerRect::HandleModelRotation(float roll, float deltaTime, glm::vec3 axis) -> void {
  if (_canMove) {
    if (roll != 0.f) {
      if ((abs(_modelTransform->Rotation().x) < _playerSettings->MaxMoveRotation() && axis.x != 0.f)
          || (abs(_modelTransform->Rotation().y) < _playerSettings->MaxMoveRotation()
              && axis.y != 0.f)) {
        _modelTransform->Rotate(roll * deltaTime, axis);
      }
    } else {
      auto currentRot = _modelTransform->Rotation();
      auto desiredRot =
          glm::quat(1.f, axis.x == 0.f ? currentRot.x : -0.11f, axis.y == 0.f ? currentRot.y : 0,
                    axis.z == 0.f ? currentRot.z : 0);

      _modelTransform->Rotation(glm::lerp(currentRot, desiredRot, 0.05f));
    }
  }
}

auto PlayerRect::ModelRotation(float vertical, float horizontal, float deltaTime) -> void {
  auto v_target = (vertical > 0 ? _qUp : (vertical < 0 ? _qDown : _qIden));
  auto h_target = (horizontal > 0 ? _qRight : (horizontal < 0 ? _qLeft : _qIden));

  if (_vLerp.End() != v_target) {
    /* Get ratio to scale lerp time up */
    auto v_rad = glm::pitch(_vLerp.Value());
    auto t_rad = glm::pitch(v_target);
    auto ratio = glm::abs(v_rad - t_rad) / _vRad;
    ratio      = glm::max(0.001f, ratio);

    _vLerp.Set(_vLerp.Value(), v_target, _lerpTime * ratio);
  }
  if (_hLerp.End() != h_target) {
    /* Get ratio to scale lerp time up */
    auto h_rad = glm::yaw(_hLerp.Value());
    auto t_rad = glm::yaw(h_target);
    auto ratio = glm::abs(h_rad - t_rad) / _hRad;
    ratio      = glm::max(0.001f, ratio);
    _hLerp.Set(_hLerp.Value(), h_target, _lerpTime * ratio);
  }
  auto rotation = _qBase * _vLerp.Update(deltaTime) * _hLerp.Update(deltaTime);
  _modelTransform->Rotation(rotation);
}

auto PlayerRect::GetNode() -> std::shared_ptr< Engine::Node > {
  const auto& node_tr = EntityManager::GetComponent< Engine::Transform >(_currentNode->GetEntity());
  const auto delta    = node_tr->WorldPosition() - _transform->Position();
  const auto magnitude2 = glm::dot(delta, delta);
  auto min_mag          = _playerSettings->MinNodeDistance();
  if (magnitude2 <= (min_mag * min_mag)) {
    // if (_currentNode->NextIndex() == 3)
    // GameManager::ShowLevelSumUp(10.0f, true);
    _currentNode   = _nodeSystem->GetNode(_currentNode->NextIndex(), NodeTag::Player);
    LOG_DEBUG("Current node: " + std::to_string(_currentNode->Index()));
    _nodeTransform = EntityManager::GetComponent< Engine::Transform >(_currentNode->GetEntity());
  }
  return _currentNode;
}
