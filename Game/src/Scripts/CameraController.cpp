#include "CameraController.h"

CameraController::CameraController(const std::shared_ptr< Engine::Transform >& player_transform)
    : _playerTransform(player_transform) {
}

auto CameraController::OnCreate() -> void {
  _cameraTransform = Entity()->GetComponent< Engine::Transform >();
}

auto CameraController::Update(float deltaTime) -> void {
  auto x        = _playerTransform->Right() * _offset.x;
  auto y        = _playerTransform->Up() * _offset.y;
  auto z        = _playerTransform->Forward() * _offset.z;
  auto offset   = x + y + z;
  auto position = _cameraTransform->Position(_playerTransform->Position() + offset);
  auto look_dir = glm::normalize(_playerTransform->Position() - position);
  _cameraTransform->Forward(look_dir);
}
