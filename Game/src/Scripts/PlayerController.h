#pragma once
#include <Engine.h>

struct ImageData
{
  std::shared_ptr<Engine::Transform> transform;
  std::shared_ptr<Engine::Renderer::Image> image;
};

class PlayerController : public Engine::Script {
private:
  float _speed             = 0.5f;
  float _minNodeDistance   = 0.2f;
  float _seekSpeed         = 1.0f;
  glm::vec3 _moveVelocity = {0.0f, 0.0f, 1.0f};
  std::shared_ptr< Engine::Transform > _playerTransform;
  std::shared_ptr< ImageData > _image;//TODO: test, put player bar here
  std::shared_ptr< Engine::NodeSystem > _nodeSystem;
  std::shared_ptr< Engine::Node > _currentNode;
  std::shared_ptr< Engine::Transform > _nodeTransform;

public:
  PlayerController(const std::shared_ptr< Engine::Transform >& player_transform);
  auto OnCreate() -> void override;
  auto Update(float deltaTime) -> void override;

private:
  auto SeekTarget(float deltaTime) -> void;
  auto GetNode() -> std::shared_ptr< Engine::Node >;
};