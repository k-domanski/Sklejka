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
  //_playerTransform = GameManager::GetCurrentPlayerModel()->GetComponent< Engine::Transform >();
  _playerTransform = GameManager::GetCurrentPlayer()->GetComponent< Engine::Transform >();
  _maxDistance = GameManager::GetPlayerSettings()->ThrowDistance();
  _lastStateA  = false;
  _timeout     = 1.0f;

  auto entity   = Engine::ECS::EntityManager::GetInstance().CreateEntity();
  auto renderer = entity->AddComponent< Engine::Components::UIRenderer >();
  _transform    = entity->AddComponent< Engine::Transform >();
  _bar          = std::make_shared< Engine::Renderer::Bar >();
  _image        = std::make_shared< Engine::Renderer::Image >();
  renderer->AddElement(_bar);
  renderer->AddElement(_image);
  stbi_set_flip_vertically_on_load(true);
  _bar->BackgroundTexture(
      Engine::AssetManager::GetTexture2D("./textures/UI/energy_background_ver.png"));
  _bar->FillTexture(Engine::AssetManager::GetTexture2D("./textures/UI/energy_fill_zoladz_ver.png"));
  _bar->FillRatio(glm::min(1.0f, _currentTimeout / _timeout));
  _bar->Horizontal(false);
  _bar->Middle(false);
  _transform->Position(glm::vec3(1500.0f, 350.0f, 0.0f));
  _bar->BackgroundColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
  _bar->FillColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
  _bar->Size(glm::vec2(55.0f, 400.0f));
  _bar->Padding(glm::vec2(10.0f, 10.0f));

  _image->Texture(Engine::AssetManager::GetTexture2D("./textures/UI/zoladz_ic.png"));
  _image->Size({55.0f, 53.0f});
  _image->Offset({0.0f, 250.0f});
  stbi_set_flip_vertically_on_load(false);
}

auto AcornThrower::ThrowDirection() -> glm::vec3 {
  return _playerTransform->Forward();
  /*return glm::angleAxis(glm::radians(17.0f), _playerTransform->Right())
   * _playerTransform->Forward();*/
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

auto AcornThrower::Update(float deltaTime) -> void {
  if (GameManager::IsPaused()) {
    return;
  }

  if (_currentTimeout >= 0.f) {
    _currentTimeout -= deltaTime;
    _bar->FillRatio(glm::min(1.0f, (_timeout - _currentTimeout) / _timeout));
    return;
  }
  auto currentStataA = Engine::Input::IsGamepadButtonPressed(Engine::GamepadCode::BUTTON_A);
  if (currentStataA && !_lastStateA) {
    Throw(ThrowDirection());
    // TryThrow();
  }
  _lastStateA = currentStataA;
}

auto AcornThrower::OnKeyPressed(Engine::Key key) -> void {
  if (key == Engine::Key::SPACE && _currentTimeout <= 0.f) {
    Throw(ThrowDirection());
  }
}

auto AcornThrower::Throw(std::shared_ptr< Engine::Transform > target) -> void {
  /*auto acorn = Engine::ECS::EntityManager::GetInstance().CreateEntity();
  acorn->LoadFromJson("./Assets/prefabs/acorn.prefab", true);

  acorn->layer.SetState(Engine::LayerMask::Flag::Acorn);
  acorn->collisionLayer.SetState(Engine::LayerMask::Flag::Default);*/

  auto acorn = GameManager::CreateAcorn();

  acorn->GetComponent< Engine::Transform >()->Position(_playerTransform->WorldPosition());
  auto acorn_rb            = acorn->GetComponent< Engine::Components::Rigidbody >();
  auto acorn_native_script = acorn->AddComponent< Engine::NativeScript >();
  GameManager::GetSoundEngine()->play2D("./Assets/sounds/throw.wav");
  acorn_native_script->Attach< CollisionDetector >();
  acorn_rb->SetVelocity(glm::normalize(target->WorldPosition() - _playerTransform->WorldPosition())
                        * 20.f);
  _currentTimeout = _timeout;
}

auto AcornThrower::Throw(glm::vec3 direction) -> void {
  auto acorn = GameManager::CreateAcorn();

  acorn->GetComponent< Engine::Transform >()->Position(_playerTransform->WorldPosition());
  auto acorn_rb = acorn->GetComponent< Engine::Components::Rigidbody >();
  /*auto acorn_native_script = acorn->AddComponent< Engine::NativeScript >();
  acorn_native_script->Attach< CollisionDetector >();*/
  GameManager::GetSoundEngine()->play2D("./Assets/sounds/throw.wav");
  acorn_rb->SetVelocity(direction * 20.f);
  _currentTimeout = _timeout;
}
