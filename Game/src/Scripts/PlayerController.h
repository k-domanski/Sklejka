#pragma once
#include <Engine.h>

struct ImageData
{
  std::shared_ptr<Engine::Transform> transform;
  std::shared_ptr<Engine::Renderer::Image> image;
};

class PlayerController : public Engine::Script {
private:
  float _speed = 0.1f;
  std::shared_ptr< Engine::Transform > _playerTransform;
  std::shared_ptr< ImageData > _image;//TODO: test, put player bar here

public:
  PlayerController(const std::shared_ptr< Engine::Transform >& player_transform);
  auto OnCreate() -> void override;
  auto Update(float deltaTime) -> void override;
};