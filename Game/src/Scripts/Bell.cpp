#include "Bell.h"
#include <GameManager.h>

using namespace Engine;

auto Bell::OnCreate() -> void {
  _timeout          = 4.0f;
  _remainingTimeout = 0.0f;
  _turnTime         = 0.2f;
  _transform        = Entity()->GetComponent< Transform >();
  _baseRotation     = _transform->Rotation();
  _turnRotation = _baseRotation * glm::angleAxis(glm::radians(90.0f), glm::vec3{0.0, 0.0, 1.0f});
  _quatLerp.Set(_baseRotation, _baseRotation, 1.0f);
}

auto Bell::Update(float deltaTime) -> void {
  auto time_scale = GameManager::GetGameSettings()->GameTimeScale();

  /* Timeout */
  if (_remainingTimeout > 0.0f) {
    _remainingTimeout -= deltaTime * time_scale;
    if (_remainingTimeout <= 0.0f) {
      TurnBell(_turnRotation, _baseRotation, 0);
    }
  }

  /* Apply rotation */
  _transform->Rotation(_quatLerp.Update(deltaTime * time_scale));
}

auto Bell::OnCollisionEnter(const std::shared_ptr< Engine::Components::Collider >& collider)
    -> void {
  if (_remainingTimeout <= 0.0f) {
    TurnBell(_baseRotation, _turnRotation, _timeout);
  }
}

auto Bell::IsActive() -> bool {
  return _remainingTimeout < 0.0f;
}

auto Bell::TurnBell(glm::quat start, glm::quat end, float timeout) -> void {
  _remainingTimeout = timeout;
  _quatLerp.Set(start, end, _turnTime);
}
