#include "AcornThrower.h"

#include "CollisionDetector.h"
#include "GameManager.h"
#include "Systems/SceneGraph.h"
#include "Components/NativeScript.h"
#include "Components/Rigidbody.h"

auto AcornThrower::OnCreate() -> void {
  auto scene       = Engine::SceneManager::GetCurrentScene();
  auto sg          = scene->SceneGraph();
  auto bell_parent = scene->FindEntity("DZWONY");
  _bells           = sg->GetChildren(bell_parent);
  _playerTransform = GameManager::GetCurrentPlayer()->GetComponent< Engine::Transform >();
  _maxDistance     = GameManager::GetPlayerSettings()->ThrowDistance();
}

auto AcornThrower::Update(float deltaTime) -> void {
  if (_currentTimeout >= 0.f) {
    _currentTimeout -= deltaTime;
  }

  if (Engine::Input::IsGamepadButtonPressed(Engine::GamepadCode::BUTTON_A))
  {
    TryThrow();
  }

}

auto AcornThrower::TryThrow() -> void {
  if (_boss == nullptr) {
    _boss          = Engine::SceneManager::GetCurrentScene()->FindEntity("Boss");
    _bossTransform = _boss->GetComponent< Engine::Transform >();
  }
  auto bossDistance =
      glm::distance(_playerTransform->WorldPosition(), _bossTransform->WorldPosition());

  LOG_DEBUG("Distance to boss is " + std::to_string(bossDistance));

  if (bossDistance <= _maxDistance) {
    LOG_DEBUG("Boss is close enough");
    Throw(_bossTransform);
    return;
  }

  for (auto bell : _bells) {
    auto bellTransform = bell->GetComponent< Engine::Transform >();
    auto distance =
        glm::distance(_playerTransform->WorldPosition(), bellTransform->WorldPosition());
    if (distance <= _maxDistance) {
      LOG_DEBUG("Found bell that is close enough");
      Throw(bellTransform);
      // GameManager::PlayerSpeedUp();
    }
  }
}

auto AcornThrower::OnKeyPressed(Engine::Key key) -> void {

  if (key == Engine::Key::SPACE && _currentTimeout <= 0.f)
  {
    TryThrow();
  }
}

auto AcornThrower::Throw(std::shared_ptr<Engine::Transform> target) -> void
{
  auto acorn = Engine::ECS::EntityManager::GetInstance().CreateEntity();
  acorn->LoadFromJson("./Assets/prefabs/acorn.prefab", true);

  acorn->layer.SetState(Engine::LayerMask::Flag::Acorn);
  acorn->collisionLayer.SetState(Engine::LayerMask::Flag::Default);

  acorn->GetComponent< Engine::Transform >()->Position(_playerTransform->WorldPosition());
  auto acorn_rb            = acorn->GetComponent< Engine::Components::Rigidbody >();
  auto acorn_native_script = acorn->AddComponent< Engine::NativeScript >();
  GameManager::GetSoundEngine()->play2D("./Assets/sounds/throw.wav");
  acorn_native_script->Attach< CollisionDetector >();
  acorn_rb->SetVelocity(
      glm::normalize(target->WorldPosition() - _playerTransform->WorldPosition()) * 20.f);
  _currentTimeout = _timeout;
}
