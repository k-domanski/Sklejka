#pragma once
#include "Engine.h"
#include "GoldenAcorn.h"
#include "Settings/PlayerSettings.h"
#include "PlayerRect.h"

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
  glm::vec3 _moveVelocity{0};
  glm::vec3 _offset{0.f, .05f, .0f};

  float _getAcornTimer = 1.3f;
  float _flightTime    = 3.5f;

  bool _canMove = false;

  float _speedUpDuration        = 1.5f;
  float _currentSpeedUpDuration = 0.f;

  auto SeekTarget(float deltaTime) -> void;
  auto HandleMove(float deltaTime) -> void;
  auto GetNode() -> std::shared_ptr< Engine::Node >;

public:
  SecondWeasel(std::shared_ptr< GoldenAcorn > goldenAcorn);
  auto OnCreate() -> void override;
  auto StartCutscene(int startNode) -> void;
  auto Update(float deltaTime) -> void override;
  auto OnKeyPressed(Engine::Key key) -> void override;
};
