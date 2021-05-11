#pragma once
#include <Engine.h>

class CameraController : public Engine::Script {
private:
  std::shared_ptr< Engine::Transform > _playerTransform;
  std::shared_ptr< Engine::Transform > _cameraTransform;
  glm::vec3 _offset{0.0f, 0.1f, -0.5f};

public:
  CameraController(const std::shared_ptr< Engine::Transform >& player_transform);
  auto OnCreate() -> void override;
  auto Update(float deltaTime) -> void override;
};