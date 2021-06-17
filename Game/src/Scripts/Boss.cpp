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

auto Boss::UpdateSound() -> void {
  if (_jetpackSound == nullptr) {
    return;
  }
  if (_jetpackSound->getIsPaused()) {
    _jetpackSound->setIsPaused(false);
  }
  auto sound_engine = GameManager::GetSoundEngine();
  auto pos          = _transform->WorldPosition();
  auto player_pos   = _playerTransform->WorldPosition();
  auto dir          = _playerTransform->Forward();
  sound_engine->setListenerPosition({player_pos.x, player_pos.y, player_pos.z},
                                    {dir.x, dir.y, dir.z});
  _jetpackSound->setPosition({pos.x, pos.y, pos.z});
}

auto Boss::StopSound() -> void {
  if (_jetpackSound) {
    _jetpackSound->stop();
    _jetpackSound->drop();
    _jetpackSound = nullptr;
  }
}

Boss::Boss(std::shared_ptr< PlayerRect > player, std::shared_ptr< GoldenAcorn > goldenAcorn)
    : _player(player), _goldenAcorn(goldenAcorn), _distState(Distance::TooClose) {
}

auto Boss::OnCreate() -> void {
  _transform       = Entity()->GetComponent< Engine::Transform >();
  _playerTransform = _player->Entity()->GetComponent< Transform >();
  _rigidbody       = Entity()->GetComponent< Components::Rigidbody >();
  _nodeSystem      = ECS::EntityManager::GetInstance().GetSystem< NodeSystem >();
  _currentNode     = _nodeSystem->GetNode(1, NodeTag::Boss);
  _nodeTransform   = EntityManager::GetComponent< Engine::Transform >(_currentNode->GetEntity());
  _playerSettings  = GameManager::GetPlayerSettings();
  _bossShowUp      = false;

  auto entity    = EntityManager::GetInstance().CreateEntity();
  _renderer      = entity->AddComponent< Components::UIRenderer >();
  auto transform = entity->AddComponent< Transform >();
  transform->Position({800.0f, 800.0f, 0.0f});
  stbi_set_flip_vertically_on_load(true);
  _weaselImage = std::make_shared< Renderer::Image >();
  _weaselImage->Texture(AssetManager::GetTexture2D("./textures/UI/lasica_ic.png"));
  _weaselImage->Size({56.0f, 50.0f});
  _weaselImage->Offset({-125.0f, 0.0f});

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

  _renderer->AddElement(_weaselImage);
  _renderer->AddElement(_health1);
  _renderer->AddElement(_health2);
  _renderer->AddElement(_health3);

  _weaselImage->SetActive(false);
  _health1->SetActive(false);
  _health2->SetActive(false);
  _health3->SetActive(false);

  auto pos      = _transform->Position();
  _jetpackSound = GameManager::GetSoundEngine()->play3D("./Assets/sounds/jetpack.wav",
                                                        {pos.x, pos.y, pos.z}, true, true, true);
  _jetpackSound->setMinDistance(12.0f);
  UpdateSound();
  _jetpackSound->setIsPaused(true);
  GameManager::AddSound(_jetpackSound);

  _minDistToPlayer = 3.0f;
  _maxDistToPlayer = 40.0f;
  _distState       = Distance::TooFar;
  _lerpTime        = 0.5f;

  _speedLerp.Set(_playerSettings->ForwardSpeedBase(), _playerSettings->ForwardSpeedBase(), 1.0f);
}

auto Boss::Update(float deltaTime) -> void {
  if (GameManager::IsPaused()) {
    return;
  }

  _canMove = _player->CurrentNodeIndex() > 89;

  /*if (_dotProduct < 0.f && _distanceToPlayer > 20.f) {
    _playerSettings->BossForwardSpeed(_playerSettings->ForwardSpeedBase());
  } else if (_currentSpeedUpDuration <= 0.f) {
    _playerSettings->BossForwardSpeed(_playerSettings->BossForwardSpeedBase());
  }*/

  if (_canMove) {
    if (!_bossShowUp) {
      _weaselImage->SetActive(true);
      _health1->SetActive(true);
      _health2->SetActive(true);
      _health3->SetActive(true);
      _bossShowUp = true;
    }

    UpdateSpeed(deltaTime);
    SeekTarget(deltaTime);
    HandleMove(deltaTime);

    UpdateSound();
  }

  /*if (_currentSpeedUpDuration > 0.f) {
    _currentSpeedUpDuration -= deltaTime;
    if (_currentSpeedUpDuration <= 0.f) {
      _playerSettings->BossForwardSpeed(_playerSettings->BossForwardSpeedBase());
    }
  }*/
}

auto Boss::UpdateSpeed(float deltaTime) -> void {
  _distanceToPlayer =
      glm::distance2(_transform->WorldPosition(), _playerTransform->WorldPosition());
  _dotProduct =
      glm::dot(_transform->Forward(),
               glm::normalize(_transform->WorldPosition() - _playerTransform->WorldPosition()));

  auto is_behind_player = _dotProduct > 0.0f;
  auto is_too_far       = _distanceToPlayer > (_maxDistToPlayer * _maxDistToPlayer);
  auto is_too_close     = _distanceToPlayer < (_minDistToPlayer * _minDistToPlayer);

  auto new_dist_state = Distance::InRange;
  if (is_too_close) {
    new_dist_state = Distance::TooClose;
  } else if (is_too_far) {
    new_dist_state = Distance::TooFar;
  } else if ((is_behind_player && !is_too_far)) {
    new_dist_state = Distance::Behind;
  }

  if (_distState != +new_dist_state) {
    _distState = new_dist_state;
    /* Handle speed change*/
    auto current_speed     = _playerSettings->BossForwardSpeed();
    auto player_base_speed = _playerSettings->ForwardSpeedBase();
    auto player_speed      = _playerSettings->ForwardSpeed();
              glm::sqrt(_distanceToPlayer), _dotProduct);
    switch (_distState) {
      case Distance::InRange: {
        _speedLerp.Set(current_speed, player_base_speed * 1.2f, _lerpTime);
        break;
      }
      case Distance::TooFar: {
        _speedLerp.Set(current_speed, player_base_speed * 0.75f, _lerpTime);
        break;
      }
      case Distance::Behind:
      case Distance::TooClose: {
        _speedLerp.Set(current_speed, player_speed * 1.2f, _lerpTime);
        break;
      }
    }
  }

  auto time_scale = GameManager::GetGameSettings()->GameTimeScale();
  _playerSettings->BossForwardSpeed(_speedLerp.Update(deltaTime * time_scale));
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
  GameManager::GetSoundEngine()->play2D("./Assets/sounds/boss_hit.wav");
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
    //_rigidbody->SetVelocity(glm::vec3(.0f, -2.5f, 0.f));
    SceneManager::GetCurrentScene()
        ->FindEntity("Boss2")
        ->GetComponent< NativeScript >()
        ->GetScript< SecondWeasel >()
        ->StartCutscene(_currentNode->Index() - 2);
    // Engine::ECS::EntityManager::GetInstance().RemoveEntity(Entity());
    // GameManager::Win();
    StopSound();
  }
}

auto Boss::SeekTarget(float deltaTime) -> void {
  auto forward_speed = _playerSettings->ForwardSpeed();
  auto ratio      = _playerSettings->BossForwardSpeed() / _playerSettings->BossForwardSpeedBase();
  auto seek_speed = _playerSettings->SeekSpeed() * ratio;
  auto node       = GetNode();
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
  auto new_pos = _transform->Position()
                 + glm::normalize(_moveVelocity) * _playerSettings->BossForwardSpeed() * deltaTime
                       * GameManager::GetGameSettings()->PlayerTimeScale()
                       * GameManager::GetGameSettings()->GameTimeScale();
  if (!_killed)
    _transform->Position(new_pos);
  else
    _transform->Position(
        glm::vec3(new_pos.x, _transform->Position().y - 0.1f * deltaTime, new_pos.z));
}

auto Boss::SpeedUp() -> void {
  auto base_speed    = _playerSettings->ForwardSpeedBase() * _playerSettings->SpeedMultiplier();
  auto current_speed = _speedLerp.Value();
  _speedLerp.Set(current_speed, base_speed * 1.2f, _lerpTime);

  //_playerSettings->BossForwardSpeed(_playerSettings->BossForwardSpeedBase()
  //                                  * _playerSettings->BossSpeedMultiplier());
  //_currentSpeedUpDuration = _speedUpDuration;
}
