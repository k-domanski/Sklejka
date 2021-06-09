#include "BellThrower.h"

#include "CollisionDetector.h"
#include "GameManager.h"
#include "Systems/SceneGraph.h"
#include "Components/NativeScript.h"
#include "Components/Rigidbody.h"

auto BellThrower::OnCreate() -> void {
  auto scene       = Engine::SceneManager::GetCurrentScene();
  auto sg          = scene->SceneGraph();
  auto bell_parent = scene->FindEntity("DZWONY");
  _bells           = sg->GetChildren(bell_parent);
  _playerTransform = GameManager::GetCurrentPlayer()->GetComponent< Engine::Transform >();
  _maxDistance     = 15;  // TODO: Move to settings
}

auto BellThrower::Update(float deltaTime) -> void {
  if (_currentTimeout >= 0.f) {
    _currentTimeout -= deltaTime;
  }
}

auto BellThrower::OnKeyPressed(Engine::Key key) -> void {
  if (key == Engine::Key::T && _currentTimeout <= 0.f) {
    for (auto bell : _bells) {
      auto bellTransform = bell->GetComponent< Engine::Transform >();
      auto distance =
          glm::distance(_playerTransform->WorldPosition(), bellTransform->WorldPosition());
      if (distance <= _maxDistance) {
        LOG_DEBUG("Found bell that is close enough");
        auto acorn = Engine::ECS::EntityManager::GetInstance().CreateEntity();
        acorn->LoadFromJson("./Assets/prefabs/acorn.prefab");
        acorn->GetComponent< Engine::Transform >()->Position(_playerTransform->WorldPosition());
        auto acorn_rb            = acorn->GetComponent< Engine::Components::Rigidbody >();
        auto acorn_native_script = acorn->AddComponent< Engine::NativeScript >();
        GameManager::GetSoundEngine()->play2D("./Assets/sounds/throw.wav");
        acorn_native_script->Attach< CollisionDetector >();
        acorn_rb->SetVelocity((bellTransform->WorldPosition() - _playerTransform->WorldPosition())
                              * 10.f);
        _currentTimeout = _timeout;
        // GameManager::PlayerSpeedUp();
      } else {
      }
    }
  }
}
