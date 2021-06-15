#pragma once
#include "Engine.h"
#include "Settings/PlayerSettings.h"
#include "PlayerRect.h"

namespace Engine {
  namespace Components {
    class UIRenderer;
  }
}  // namespace Engine

class Boss : public Engine::Script< Boss > {
private:
  std::shared_ptr< Engine::Transform > _transform;
  std::shared_ptr< Engine::NodeSystem > _nodeSystem;
  std::shared_ptr< Engine::Node > _currentNode;
  std::shared_ptr< Engine::Transform > _nodeTransform;
  std::shared_ptr< PlayerSettings > _playerSettings;
  std::shared_ptr< PlayerRect > _player;
  glm::vec3 _moveVelocity{0};
  std::shared_ptr< Engine::Components::UIRenderer > _renderer;
  std::shared_ptr< Engine::Renderer::Image > _health1;
  std::shared_ptr< Engine::Renderer::Image > _health2;
  std::shared_ptr< Engine::Renderer::Image > _health3;

  float _speedUpDuration = 1.5f;
  float _currentSpeedUpDuration = 0.f;
  int _hits = 0;
  bool _canMove;
  bool _bossShowUp;

  auto SeekTarget(float deltaTime) -> void;
  auto HandleMove(float deltaTime) -> void;
  auto SpeedUp() -> void;
  auto GetNode() -> std::shared_ptr< Engine::Node >;

public:
  Boss(std::shared_ptr< PlayerRect > player);
  auto OnCreate() -> void override;
  auto Update(float deltaTime) -> void override;
  auto OnKeyPressed(Engine::Key key) -> void override;
  auto CanMove() -> bool;
  auto CanMove(bool value) -> void;
  auto Hit() -> void;
};
