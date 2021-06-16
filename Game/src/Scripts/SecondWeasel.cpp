#include "SecondWeasel.h"
#include "Systems/NodeSystem.h"
#include "Systems/SceneGraph.h"
#include "GameManager.h"

auto SecondWeasel::SeekTarget(float deltaTime) -> void
{
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

auto SecondWeasel::HandleMove(float deltaTime) -> void {
  auto new_pos =
      _transform->Position()
      + glm::normalize(_moveVelocity) * _playerSettings->BossForwardSpeed() * 1.2f * deltaTime;
  _transform->Position(new_pos + _offset);
}

auto SecondWeasel::GetNode() -> std::shared_ptr<Engine::Node>
{
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

SecondWeasel::SecondWeasel(std::shared_ptr< GoldenAcorn > goldenAcorn)
    : _goldenAcorn(goldenAcorn) {
}

auto SecondWeasel::OnCreate() -> void
{
  _transform      = Entity()->GetComponent< Engine::Transform >();
  _nodeSystem     = Engine::ECS::EntityManager::GetInstance().GetSystem< Engine::NodeSystem >();
  _currentNode    = _nodeSystem->GetNode(1, Engine::NodeTag::Boss);
  _nodeTransform  = Engine::ECS::EntityManager::GetComponent< Engine::Transform >(_currentNode->GetEntity());
  _playerSettings = GameManager::GetPlayerSettings();

}

auto SecondWeasel::StartCutscene(int startNode) -> void
{
  _currentNode = _nodeSystem->GetNode(startNode - 2, Engine::NodeTag::Boss);
  _transform->Position(_currentNode->GetEntity()->GetComponent< Engine::Transform >()->WorldPosition());
  _canMove = true;
}


auto SecondWeasel::Update(float deltaTime) -> void
{
  if (_canMove) {
    SeekTarget(deltaTime);
    HandleMove(deltaTime);

    if (_getAcornTimer > 0.f) {
      _getAcornTimer -= deltaTime;
      if (_getAcornTimer <= 0.f) {
        _goldenAcorn->DetachFromBoss();
        Engine::SceneManager::GetCurrentScene()->SceneGraph()->SetParent(_goldenAcorn->Entity(),
                                                                         Entity());
        _goldenAcorn->ResetLocalPos();
      }
    }

    if (_flightTime > 0.f) {
      _flightTime -= deltaTime;
      if (_flightTime <= 0.f)
        GameManager::Win();
    }
  }
}

auto SecondWeasel::OnKeyPressed(Engine::Key key) -> void {}
