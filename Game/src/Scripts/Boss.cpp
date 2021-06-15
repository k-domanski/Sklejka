#include "Boss.h"
#include <Systems/NodeSystem.h>
#include <GameManager.h>

#include "SecondWeasel.h"
#include "Components/NativeScript.h"

#include "Components/Rigidbody.h"

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

Boss::Boss(std::shared_ptr< PlayerRect > player, std::shared_ptr< GoldenAcorn > goldenAcorn)
    : _player(player), _goldenAcorn(goldenAcorn) {
}

auto Boss::OnCreate() -> void {
  _transform      = Entity()->GetComponent< Engine::Transform >();
  _rigidbody      = Entity()->GetComponent< Components::Rigidbody >();
  _nodeSystem     = ECS::EntityManager::GetInstance().GetSystem< NodeSystem >();
  _currentNode    = _nodeSystem->GetNode(1, NodeTag::Boss);
  _nodeTransform  = EntityManager::GetComponent< Engine::Transform >(_currentNode->GetEntity());
  _playerSettings = GameManager::GetPlayerSettings();
  _bossShowUp     = false;

  auto entity    = EntityManager::GetInstance().CreateEntity();
  _renderer      = entity->AddComponent< Components::UIRenderer >();
  auto transform = entity->AddComponent< Transform >();
  transform->Position({800.0f, 750.0f, 0.0f});
  stbi_set_flip_vertically_on_load(true);
  _health1 = std::make_shared< Renderer::Image >();
  _health1->Texture(AssetManager::GetTexture2D("./textures/UI/heart_full.png"));
  stbi_set_flip_vertically_on_load(false);
  _health1->Size({56.0f, 50.0f});
  _health1->Offset({60.0f, 0.0f});

  _health2 = std::make_shared< Renderer::Image >();
  _health2->Texture(AssetManager::GetTexture2D("./textures/UI/heart_full.png"));
  _health2->Size({56.0f, 50.0f});
  _health2->Offset({0.0f, 0.0f});

  _health3 = std::make_shared< Renderer::Image >();
  _health3->Texture(AssetManager::GetTexture2D("./textures/UI/heart_full.png"));
  _health3->Size({56.0f, 50.0f});
  _health3->Offset({-60.0f, 0.0f});

  _renderer->AddElement(_health1);
  _renderer->AddElement(_health2);
  _renderer->AddElement(_health3);

  _health1->SetActive(false);
  _health2->SetActive(false);
  _health3->SetActive(false);
}

auto Boss::Update(float deltaTime) -> void {
  if (!_killed) {
    _canMove = _player->CurrentNodeIndex() > 89;

    if (_canMove) {
      if (!_bossShowUp) {
        _health1->SetActive(true);
        _health2->SetActive(true);
        _health3->SetActive(true);
        _bossShowUp = true;
      }

      SeekTarget(deltaTime);
      HandleMove(deltaTime);
    }
  }

  if (_currentSpeedUpDuration > 0.f) {
    _currentSpeedUpDuration -= deltaTime;
    if (_currentSpeedUpDuration <= 0.f) {
      _playerSettings->BossForwardSpeed(_playerSettings->BossForwardSpeedBase());
    }
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

auto Boss::Hit() -> void {
  _hits++;
  GameManager::GetSoundEngine()->play2D("./Assets/sounds/placeholderBeep.wav");
  SpeedUp();

  switch (_hits) {
    case 1:
      stbi_set_flip_vertically_on_load(true);
      _health1->Texture(AssetManager::GetTexture2D("./textures/UI/heart_empty.png"));
      stbi_set_flip_vertically_on_load(false);
      break;
    case 2:
      _health2->Texture(AssetManager::GetTexture2D("./textures/UI/heart_empty.png"));
      break;
    case 3:
      _health3->Texture(AssetManager::GetTexture2D("./textures/UI/heart_empty.png"));
      break;
  }

  if (_hits >= 3) {
    _goldenAcorn->OnBossKilled();
    _killed = true;
    _rigidbody->SetGravity(true);
    _rigidbody->SetKinematic(true);
    _rigidbody->SetVelocity(glm::vec3(.0f, -5.f, 0.f));
    SceneManager::GetCurrentScene()
        ->FindEntity("Boss2")
        ->GetComponent< NativeScript >()
        ->GetScript< SecondWeasel >()
        ->StartCutscene(_currentNode->Index());
    // Engine::ECS::EntityManager::GetInstance().RemoveEntity(Entity());
    // GameManager::Win();
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

auto Boss::SpeedUp() -> void {
  _playerSettings->BossForwardSpeed(_playerSettings->BossForwardSpeedBase()
                                    * _playerSettings->BossSpeedMultiplier());
  _currentSpeedUpDuration = _speedUpDuration;
}
