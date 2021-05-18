#include "CameraController.h"

CameraController::CameraController(const std::shared_ptr< PlayerController >& player_controller)
    : _playerController(player_controller) {
}

auto CameraController::OnCreate() -> void {
  _cameraTransform = Entity()->GetComponent< Engine::Transform >();
}

auto CameraController::Update(float deltaTime) -> void {
  auto player_transform = _playerController->Transform();

  auto x        = player_transform->Right() * _offset.x;
  auto y        = player_transform->Up() * _offset.y;
  auto z        = player_transform->Forward() * _offset.z;
  auto offset   = x + y + z;
  auto position = _cameraTransform->Position(player_transform->WorldPosition() + offset);
  auto look_dir = glm::normalize(player_transform->WorldPosition() - position);
  _cameraTransform->Forward(look_dir);
}
