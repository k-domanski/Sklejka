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
  auto entity = collider->GetEntity();
  if (entity != nullptr) {
    LOG_INFO("Collision detected: {}", entity->Name());
    const std::regex bell_rx("BELL.*");
    if (std::regex_match(entity->Name(), bell_rx)) {
      if (Entity()->Name() == "Player_Model") {
        return;
      }
      if (_timeout <= 0.0f) {
        _timeout = _timeoutDuration;
        GameManager::GetSoundEngine()->play2D("./Assets/sounds/bell.wav");
        GameManager::PlayerSpeedUp();
        Engine::ECS::EntityManager::GetInstance().RemoveEntity(Entity());
      }
    } else {
      /* Player fucking dies */
      if (Entity()->Name() != "Acorn" && entity->Name() != "Player_Model")
        GameManager::KillPlayer();
    }
  }
}
