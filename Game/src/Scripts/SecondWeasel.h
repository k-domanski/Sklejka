#pragma once
#include "Engine.h"
#include "GoldenAcorn.h"
#include "Settings/PlayerSettings.h"
#include "PlayerRect.h"
#include <Utility/Utility.h>

namespace Engine {
  namespace Components {
    class UIRenderer;
  }
}  // namespace Engine

class SecondWeasel : public Engine::Script< SecondWeasel > {
private:
  std::shared_ptr< Engine::Transform > _transform;
  std::shared_ptr< Engine::NodeSystem > _nodeSystem;
  std::shared_ptr< Engine::Node > _currentNode;
  std::shared_ptr< Engine::Transform > _nodeTransform;
  std::shared_ptr< PlayerSettings > _playerSettings;
  std::shared_ptr< PlayerRect > _player;
  std::shared_ptr< GoldenAcorn > _goldenAcorn;
  std::shared_ptr< Engine::ECS::Entity > _model;
  std::shared_ptr< Engine::Transform > _modelTransform;
  std::shared_ptr< Engine::Transform > _bossTr;

  glm::vec3 _moveVelocity{0};
  glm::vec3 _offsetBase{0.f, 1.5f, .0f};
  Engine::Utility::FloatLerp _offsetLerp;

  float _speed{1.0f};
  int _frameCounter{0};

  float _getAcornTimer = .6f;
  float _flightTime    = 5.0f;
  float _targetTime{0.0f};

  bool _continueForward = false;
  glm::vec3 _fwd;
  bool _canMove = false;

  float _speedUpDuration        = 1.5f;
  float _currentSpeedUpDuration = 0.f;

  auto SeekTarget(float deltaTime) -> void;
  auto HandleMove(float deltaTime) -> void;
  auto UpdateModel(float deltaTime) -> void;
  auto GetNode() -> std::shared_ptr< Engine::Node >;

public:
  SecondWeasel(std::shared_ptr< Engine::ECS::Entity > model,
               std::shared_ptr< GoldenAcorn > goldenAcorn);
  auto OnCreate() -> void override;
  auto StartCutscene(std::shared_ptr< PlayerRect > player,
                     std::shared_ptr< Engine::Transform > bossTr) -> void;
  auto Update(float deltaTime) -> void override;
  auto OnKeyPressed(Engine::Key key) -> void override;
  auto TargetTime() -> float;
  auto ModelTransform() -> std::shared_ptr< Engine::Transform >;
};
