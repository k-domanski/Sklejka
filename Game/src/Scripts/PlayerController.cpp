#include "PlayerController.h"

#include <GameManager.h>

using namespace Engine;
using namespace Engine::ECS;
PlayerController::PlayerController(const std::shared_ptr< Engine::Transform >& player_transform)
    : _transform(player_transform) {
}

auto PlayerController::OnCreate() -> void {
}

auto PlayerController::Update(float deltaTime) -> void {
}

auto PlayerController::OnKeyPressed(Engine::Key key) -> void {
}

auto PlayerController::Transform() const noexcept -> std::shared_ptr< Engine::Transform > {
  return _transform;
}

auto PlayerController::OnCollisionEnter(
    const std::shared_ptr< Engine::Components::Collider >& collider) -> void {
  if (auto& ent = collider->GetEntity(); ent != nullptr) {
    if (ent->layer.Get(LayerMask::Flag::Bell)) {
      /* Speed up player if we decide to */
      return;
    }
  }
  GameManager::KillPlayer();
}
