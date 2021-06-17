#include "Boss.h"
#include <Systems/NodeSystem.h>
#include <GameManager.h>

#include "SecondWeasel.h"
#include "Components/NativeScript.h"

#include "Components/Rigidbody.h"
#include <limits>

using namespace Engine;
using namespace Engine::ECS;

#define RAND_FLOAT(_min, _max)                                                                     \
  (_min + (rand() / std::numeric_limits< float >::max()) * (_max - _min))
#define RAND_VEC3(min, max)                                                                        \
  glm::vec3(RAND_FLOAT(min, max), RAND_FLOAT(min, max), RAND_FLOAT(min, max))

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
    _jetpackSound = nullptr;
  }
}

auto Boss::Kill() -> void {
  _rigidbody->SetKinematic(true);
  auto weasel = SceneManager::GetCurrentScene()
                    ->FindEntity("Boss2")
                    ->GetComponent< NativeScript >()
                    ->GetScript< SecondWeasel >();
  weasel->StartCutscene(_player, _transform);
  auto target_time = weasel->TargetTime();
  _goldenAcorn->StartTransition(_modelTransform, weasel->ModelTransform(), target_time);
  //_goldenAcorn->OnBossKilled();

  /* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

  auto current_speed     = _playerSettings->BossForwardSpeed();
  auto player_base_speed = _playerSettings->ForwardSpeedBase();
  _speedLerp.Set(current_speed, player_base_speed * 1.1f, _lerpTime);

  _offsetLerp.Set(0.0f, 1.0f, _fallTime);
  _state = BossState::Dead;

  constexpr auto r_speed = 0.25f * glm::pi< float >();
  _rot1                  = RAND_FLOAT(-r_speed, r_speed);
  _rotationAxis1         = glm::angleAxis(glm::radians(90.0f), glm::vec3{0.0f, 1.0f, 0.0f})
                   * glm::normalize(_moveVelocity);
}

Boss::Boss(std::shared_ptr< Engine::ECS::Entity > model, std::shared_ptr< PlayerRect > player,
           std::shared_ptr< GoldenAcorn > goldenAcorn)
    : _player(player), _goldenAcorn(goldenAcorn), _state(BossState::TooClose) {
  _modelTransform = model->GetComponent< Transform >();
}

auto Boss::UpdateModel(float deltaTime) -> void {
  if (_state != +BossState::Dead) {
    return;
  }

  auto time_scale = GameManager::GetGameSettings()->GameTimeScale();
  auto dt         = deltaTime * time_scale;
  auto offset     = _offsetLerp.Update(dt);
  auto offset2    = offset * offset;
  _modelTransform->Position(_heightOffset * (offset2 * offset2));
  _modelTransform->Rotate(_rot1 * dt, _rotationAxis1);
}

auto Boss::OnCreate() -> void {
  { /* Prepare Boss */
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
  }

  { /* Sound */
    auto pos      = _transform->Position();
    _jetpackSound = GameManager::GetSoundEngine()->play3D("./Assets/sounds/jetpack.wav",
                                                          {pos.x, pos.y, pos.z}, true, true, true);
    _jetpackSound->setMinDistance(12.0f);
    UpdateSound();
    _jetpackSound->setIsPaused(true);
    GameManager::AddSound(_jetpackSound);
  }

  { /* Speed */
    _speedLerp.Set(_playerSettings->ForwardSpeedBase(), _playerSettings->ForwardSpeedBase(), 1.0f);
    _minDistToPlayer = 3.0f;
    _maxDistToPlayer = 40.0f;
    _state           = BossState::TooFar;
    _lerpTime        = 0.5f;
  }

  { /* Offset */
    _heightOffset = {0.0f, -5.0f, 0.0f};
    _offsetLerp.Set(0.0f, 0.0f, 1.0f);
    _fallTime = 3.0f;
  }
}

auto Boss::Update(float deltaTime) -> void {
  if (GameManager::IsPaused()) {
    return;
  }

  _canMove = _player->CurrentNodeIndex() > 89;

  if (!_canMove) {
    return;
  }

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
  UpdateModel(deltaTime);

  UpdateSound();
}

auto Boss::UpdateSpeed(float deltaTime) -> void {
  auto is_dead = _state == +BossState::Dead;
  if (is_dead) {
    return;
  }

  _distanceToPlayer =
      glm::distance2(_transform->WorldPosition(), _playerTransform->WorldPosition());
  _dotProduct =
      glm::dot(_transform->Forward(),
               glm::normalize(_transform->WorldPosition() - _playerTransform->WorldPosition()));

  auto is_behind_player = _dotProduct > 0.0f;
  auto is_too_far       = _distanceToPlayer > (_maxDistToPlayer * _maxDistToPlayer);
  auto is_too_close     = _distanceToPlayer < (_minDistToPlayer * _minDistToPlayer);
  auto is_escaping      = _state == +BossState::Escape;

  auto new_dist_state = is_escaping ? BossState::Escape : BossState::InRange;
  if (is_too_close && !is_escaping) {
    new_dist_state = BossState::TooClose;
  } else if (is_too_far) {
    new_dist_state = BossState::TooFar;
  } else if ((is_behind_player && !is_too_far)) {
    new_dist_state = BossState::Behind;
  }

  if (_state != +new_dist_state) {
    _state = new_dist_state;
    /* Handle speed change*/
    auto current_speed     = _playerSettings->BossForwardSpeed();
    auto player_base_speed = _playerSettings->ForwardSpeedBase();
    auto player_speed      = _playerSettings->ForwardSpeed();

    switch (_state) {
      case BossState::InRange: {
        _speedLerp.Set(current_speed, player_base_speed * 1.1f, _lerpTime);
        break;
      }
      case BossState::TooFar: {
        _speedLerp.Set(current_speed, player_base_speed * 0.75f, _lerpTime);
        break;
      }
      case BossState::Behind:
      case BossState::TooClose: {
        _speedLerp.Set(current_speed, player_speed * 1.2f, _lerpTime);
        break;
      }
    }
  }

  auto time_scale = GameManager::GetGameSettings()->GameTimeScale();
  _playerSettings->BossForwardSpeed(_speedLerp.Update(deltaTime * time_scale));
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
    Kill();
    StopSound();
  }
}

auto Boss::SeekTarget(float deltaTime) -> void {
  auto forward_speed = _playerSettings->ForwardSpeed();
  auto ratio      = _playerSettings->BossForwardSpeed() / _playerSettings->BossForwardSpeedBase();
  auto seek_speed = _playerSettings->SeekSpeed() * ratio;
  auto node       = GetNode();
  auto time_scale = GameManager::GetGameSettings()->GameTimeScale();

  auto desired_velocity =
      glm::normalize(_nodeTransform->WorldPosition() - _transform->WorldPosition()) * forward_speed;
  auto velocity_delta = desired_velocity - _moveVelocity;

  auto new_velocity = _moveVelocity + velocity_delta * seek_speed * deltaTime * time_scale;
  _moveVelocity     = new_velocity;

  auto tmp = glm::lookAt(_transform->WorldPosition(), _transform->WorldPosition() + new_velocity,
                         {0.0f, 1.0f, 0.0f});
  tmp[3]   = glm::vec4(0.0f);
  tmp      = glm::transpose(tmp);
  auto rot = glm::quat_cast(tmp);
  _transform->Rotation(rot);
}

auto Boss::HandleMove(float deltaTime) -> void {
  auto time_scale = GameManager::GetGameSettings()->GameTimeScale();
  auto pos        = _transform->WorldPosition();
  auto move_delta =
      glm::normalize(_moveVelocity) * _playerSettings->BossForwardSpeed() * deltaTime * time_scale;
  auto new_pos = pos + move_delta;

  _transform->Position(new_pos);
}

auto Boss::SpeedUp() -> void {
  auto base_speed    = _playerSettings->ForwardSpeedBase() * _playerSettings->SpeedMultiplier();
  auto current_speed = _speedLerp.Value();
  _speedLerp.Set(current_speed, base_speed * 1.3f, _lerpTime);
  _state = BossState::Escape;
}
