#include "CollisionDetector.h"
#include <GameManager.h>
#include <regex>

using Engine::Components::Collider;

auto CollisionDetector::Update(float deltaTime) -> void {
  if (_timeout > 0.0f) {
    _timeout -= deltaTime;
  }
}

auto CollisionDetector::OnKeyPressed(Engine::Key key) -> void {
}

auto CollisionDetector::OnCollisionEnter(const std::shared_ptr< Collider >& collider) -> void {
  auto entity = Engine::ECS::EntityManager::GetInstance().GetEntity(collider->GetEntityID());
  if (entity != nullptr) {
    LOG_INFO("Collision detected: {}", entity->Name());
    const std::regex bell_rx("BELL.*");
    if (std::regex_match(entity->Name(), bell_rx)) {
      if (_timeout <= 0.0f) {
        _timeout = _timeoutDuration;
        GameManager::GetSoundEngine()->play2D("./Assets/sounds/bell.wav");
        GameManager::PlayerSpeedUp();
      }
    } else {
      /* Player fucking dies */
      GameManager::KillPlayer();
    }
  }
}
