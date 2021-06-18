#include "SecondWeasel.h"
#include "Systems/NodeSystem.h"
#include "Systems/SceneGraph.h"
#include "GameManager.h"

using namespace Engine;
auto SecondWeasel::SeekTarget(float deltaTime) -> void {
  auto ratio         = _playerSettings->ForwardSpeed() / _playerSettings->ForwardSpeedBase();
  auto forward_speed = _speed * ratio;
  auto seek_speed    = _playerSettings->SeekSpeed() * ratio;
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

auto SecondWeasel::HandleMove(float deltaTime) -> void {
  auto ratio = _playerSettings->ForwardSpeed() / _playerSettings->ForwardSpeedBase();
  auto pos   = _transform->WorldPosition();
  if (!_continueForward) {
    auto delta = _bossTr->WorldPosition() - pos;
    if (glm::length(delta) > 0.5f) {
      _fwd = glm::normalize(delta);

    } else {
      _continueForward = true;
    }
  }

  auto velocity = _fwd * _speed;
  auto new_pos  = pos + velocity * deltaTime * ratio;
  pos           = _transform->Position(new_pos);

  /* Rotate towards movement */
  auto tmp = glm::lookAt(pos, pos + _fwd, {0.0f, 1.0f, 0.0f});
  tmp[3]   = glm::vec4(0.0f);
  tmp      = glm::transpose(tmp);
  auto rot = glm::quat_cast(tmp);
  _transform->Rotation(rot);
}

auto SecondWeasel::UpdateModel(float deltaTime) -> void {
  auto lerp   = _offsetLerp.Update(deltaTime);
  auto offset = glm::vec3(0.0f, (lerp * lerp), 0.0f);
  _modelTransform->Position(_offsetBase + offset * 1.5f);
}

auto SecondWeasel::GetNode() -> std::shared_ptr< Engine::Node > {
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

SecondWeasel::SecondWeasel(std::shared_ptr< Engine::ECS::Entity > model,
                           std::shared_ptr< GoldenAcorn > goldenAcorn)
    : _goldenAcorn(goldenAcorn), _model(model) {
  _modelTransform = model->GetComponent< Engine::Transform >();
}

auto SecondWeasel::OnCreate() -> void {
  _transform   = Entity()->GetComponent< Engine::Transform >();
  _nodeSystem  = Engine::ECS::EntityManager::GetInstance().GetSystem< Engine::NodeSystem >();
  _currentNode = _nodeSystem->GetNode(1, Engine::NodeTag::Boss);
  _nodeTransform =
      Engine::ECS::EntityManager::GetComponent< Engine::Transform >(_currentNode->GetEntity());
  _playerSettings = GameManager::GetPlayerSettings();
  _offsetLerp.Set(-1.0f, 1.0f, _flightTime);
}

auto SecondWeasel::StartCutscene(std::shared_ptr< PlayerRect > player,
                                 std::shared_ptr< Transform > bossTr) -> void {
  _player        = player;
  _bossTr        = bossTr;
  auto start_pos = player->Entity()->GetComponent< Transform >()->WorldPosition();
  auto dir       = glm::normalize(_bossTr->WorldPosition() - start_pos);
  start_pos      = _transform->Position(start_pos - dir * 7.0f);

  auto dist   = glm::distance(start_pos, _bossTr->WorldPosition());
  _speed      = _playerSettings->ForwardSpeedBase() * 2.0f;
  _targetTime = dist / _speed;
  _offsetLerp.Set(-1.0f, 1.0f, _targetTime);
  _canMove = true;
}

auto SecondWeasel::Update(float deltaTime) -> void {
  if (_canMove) {
    if (_frameCounter > 0) {
      HandleMove(deltaTime);
      UpdateModel(deltaTime);
    } else {
      ++_frameCounter;
    }

    if (_flightTime > 0.f) {
      _flightTime -= deltaTime;
      if (_flightTime <= 0.f)
        GameManager::Win();
    }
  }
}

auto SecondWeasel::OnKeyPressed(Engine::Key key) -> void {
}

auto SecondWeasel::TargetTime() -> float {
  return _targetTime;
}

auto SecondWeasel::ModelTransform() -> std::shared_ptr< Engine::Transform > {
  return _modelTransform;
}
