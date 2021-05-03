#include "PlayerController.h"

using namespace Engine;
PlayerController::PlayerController(const std::shared_ptr< Engine::Transform >& player_transform)
    : _playerTransform(player_transform) {
}

auto PlayerController::OnCreate() -> void {
}

auto PlayerController::Update(float deltaTime) -> void {
  // auto new_pos = _playerTransform->Position() + _playerTransform->Forward() * _speed * deltaTime;
  /* Replace it with Transform Right/Up/Forward once model gets the correct pivot */
  auto move_delta = glm::vec3{0.0f, 0.0f, 1.0f};

  auto vertical_move =
      ((float)Engine::Input::IsKeyPressed(Key::W)) - ((float)Engine::Input::IsKeyPressed(Key::S));
  auto horizontal_move =
      ((float)Engine::Input::IsKeyPressed(Key::D)) - ((float)Engine::Input::IsKeyPressed(Key::A));

  move_delta += vertical_move * glm::vec3{0.0f, -1.0f, 0.0f};
  move_delta += horizontal_move * glm::vec3{-1.0f, 0.0f, 0.0f};

  _playerTransform->Position(_playerTransform->Position() + move_delta * _speed * deltaTime);
}