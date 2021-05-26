#include "CollisionDetector.h"
#include <GameManager.h>

using Engine::Components::Collider;

auto CollisionDetector::Update(float deltaTime) -> void {
  if (_timeout > 0.0f) {
    _timeout -= deltaTime;
  }
}

auto CollisionDetector::OnCollisionEnter(const std::shared_ptr< Collider >& collider) -> void {
  auto entity = Engine::ECS::EntityManager::GetInstance().GetEntity(collider->GetEntityID());
  if (entity->Name() == "Bell") {
    if (_timeout <= 0.0f) {
      LOG_INFO("Collision detected: {}", entity->Name());
      _timeout = _timeoutDuration;
      GameManager::PlayerSpeedUp();
    }
  }
}
