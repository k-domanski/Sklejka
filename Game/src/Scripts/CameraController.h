#pragma once
#include <Engine.h>
#include <Scripts/PlayerController.h>

class CameraController : public Engine::Script {
private:
  //std::shared_ptr< Engine::Transform > _playerTransform;
  std::shared_ptr< Engine::Transform > _cameraTransform;
  std::shared_ptr< PlayerController > _playerController;
  glm::vec3 _offset;

public:
  CameraController(const std::shared_ptr< PlayerController >& player_controller);
  auto OnCreate() -> void override;
  auto Update(float deltaTime) -> void override;
  auto OnKeyPressed(Engine::Key key) -> void override;
};