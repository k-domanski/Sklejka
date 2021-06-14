#pragma once
#include "Engine.h"
#include "Settings/PlayerSettings.h"
#include "PlayerRect.h"

class Boss : public Engine::Script< Boss > {
private:
  std::shared_ptr< Engine::Transform > _transform;
  std::shared_ptr< Engine::NodeSystem > _nodeSystem;
  std::shared_ptr< Engine::Node > _currentNode;
  std::shared_ptr< Engine::Transform > _nodeTransform;
  std::shared_ptr< PlayerSettings > _playerSettings;
  std::shared_ptr< PlayerRect > _player;
  glm::vec3 _moveVelocity{0};

  int _hits = 0;
  bool _canMove;

  auto SeekTarget(float deltaTime) -> void;
  auto HandleMove(float deltaTime) -> void;
  auto GetNode() -> std::shared_ptr< Engine::Node >;

public:
  Boss(std::shared_ptr<PlayerRect> player);
  auto OnCreate() -> void override;
  auto Update(float deltaTime) -> void override;
  auto OnKeyPressed(Engine::Key key) -> void override;
  auto CanMove() -> bool;
  auto CanMove(bool value) -> void;
  auto Hit() -> void;
};
