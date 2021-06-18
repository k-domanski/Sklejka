#pragma once
#include "Engine.h"
#include "GoldenAcorn.h"
#include "Settings/PlayerSettings.h"
#include "PlayerRect.h"
#include "Components/Rigidbody.h"
#include "irrKlang.h"
#include <Utility/Utility.h>

namespace Engine {
  namespace Components {
    class UIRenderer;
  }
}  // namespace Engine

BETTER_ENUM(__BossState, int, InRange, TooFar, TooClose, Behind, Escape, Dead);
typedef __BossState BossState;

class Boss : public Engine::Script< Boss > {
private:
  std::shared_ptr< Engine::Transform > _transform;
  std::shared_ptr< Engine::Components::Rigidbody > _rigidbody;
  std::shared_ptr< Engine::Transform > _playerTransform;
  std::shared_ptr< Engine::Transform > _modelTransform;
  std::shared_ptr< Engine::NodeSystem > _nodeSystem;
  std::shared_ptr< Engine::Node > _currentNode;
  std::shared_ptr< Engine::Transform > _nodeTransform;
  std::shared_ptr< PlayerSettings > _playerSettings;
  std::shared_ptr< PlayerRect > _player;
  std::shared_ptr< GoldenAcorn > _goldenAcorn;
  glm::vec3 _moveVelocity{0};
  std::shared_ptr< Engine::Components::UIRenderer > _renderer;
  std::shared_ptr< Engine::Renderer::Image > _weaselImage;
  std::shared_ptr< Engine::Renderer::Image > _health1;
  std::shared_ptr< Engine::Renderer::Image > _health2;
  std::shared_ptr< Engine::Renderer::Image > _health3;

  float _speedUpDuration        = 1.5f;
  float _currentSpeedUpDuration = 0.f;
  int _hits                     = 0;
  bool _canMove;
  int _startNodeIndex;
  bool _bossShowUp;

  /* Speed */
  float _distanceToPlayer;
  float _lerpTime;
  float _minDistToPlayer;
  float _maxDistToPlayer;
  float _dotProduct;
  BossState _state;
  Engine::Utility::FloatLerp _speedLerp;
  /* -=-=- */

  /* Offset */
  glm::vec3 _heightOffset;
  Engine::Utility::FloatLerp _offsetLerp;
  float _fallTime;
  glm::vec3 _rotationAxis1;
  float _rot1;
  /* -=-=-=-*/

  irrklang::ISound* _jetpackSound{nullptr};

  auto SeekTarget(float deltaTime) -> void;
  auto HandleMove(float deltaTime) -> void;
  auto UpdateModel(float deltaTime) -> void;
  auto SpeedUp() -> void;
  auto GetNode() -> std::shared_ptr< Engine::Node >;
  auto UpdateSound() -> void;
  auto StopSound() -> void;
  auto Kill() -> void;

public:
  Boss(std::shared_ptr< Engine::ECS::Entity > model, std::shared_ptr< PlayerRect > player,
       std::shared_ptr< GoldenAcorn > goldenAcorn);
  auto OnCreate() -> void override;
  auto Update(float deltaTime) -> void override;
  auto UpdateSpeed(float deltaTime) -> void;
  auto CanMove() -> bool;
  auto CanMove(bool value) -> void;
  auto Hit() -> void;
};
