#include "Acorn.h"
#include "GameManager.h"
#include "Boss.h"
#include "Components/NativeScript.h"

#include <regex>

#define RAND_INT(min, max) (min + (max - min) % rand())

using namespace Engine;
using namespace Engine::Components;
auto Acorn::OnCreate() -> void {
  /* Setup */
  _gravity        = 2.0f;
  _timeout        = 1.0f;
  _destroyTimeout = 0.0f;

  /* Roll initial rotaion, rotation axis, rotation speed */
  glm::vec3 axis(0.0f);
  axis.x = RAND_INT(-100, 100);
  axis.y = RAND_INT(-100, 100);
  axis.z = RAND_INT(-100, 100);
  axis   = glm::normalize(axis);

  _rotationSpeed = glm::radians((float)RAND_INT(0, 360));
  _transform     = Entity()->GetComponent< Transform >();
  _rigidbody     = Entity()->GetComponent< Rigidbody >();

  auto ang = glm::radians((float)RAND_INT(0, 360));
  _transform->Rotate(ang, axis);

  axis.x = RAND_INT(-100, 100);
  axis.y = RAND_INT(-100, 100);
  axis.z = RAND_INT(-100, 100);
  _axis  = glm::normalize(axis);
}

auto Acorn::Update(float deltaTime) -> void {
  auto time_scale = GameManager::GetGameSettings()->GameTimeScale();
  _transform->Rotate(_rotationSpeed * deltaTime * time_scale, _axis);

  /* Gravity */
  if (_timeout > 0.0f) {
    _timeout -= deltaTime;
  } else {
    auto vel = _rigidbody->GetVelocity();
    vel += glm::vec3(0.0f, -1.0f, 0.0f) * _gravity * deltaTime * time_scale;
    _rigidbody->SetVelocity(vel);
  }

  /* Timeout Destroy */
  if (_destroyTimeout > 0.0f) {
    _destroyTimeout -= deltaTime * time_scale;
    if (_destroyTimeout <= 0.0f) {
      Engine::ECS::EntityManager::GetInstance().RemoveEntity(Entity());
    }
  }
}

auto Acorn::OnCollisionEnter(const std::shared_ptr< Engine::Components::Collider >& collider)
    -> void {
  auto entity = collider->GetEntity();
  if (entity != nullptr) {
    if (entity->Name() == "Boss") {
      entity->GetComponent< Engine::NativeScript >()->GetScript< Boss >()->Hit();
    }

    const std::regex bell_rx("BELL.*");
    if (std::regex_match(entity->Name(), bell_rx)) {
      GameManager::GetSoundEngine()->play2D("./Assets/sounds/bell.wav");
      GameManager::PlayerSpeedUp();
    }
  }
  TimeoutDestroy();
  GameManager::GetSoundEngine()->play2D("./Assets/sounds/acorn_hit.wav");
}

auto Acorn::TimeoutDestroy() -> void {
  _destroyTimeout = 1.0f;
  Entity()->RemoveComponent< MeshRenderer >();
  Entity()->RemoveComponent< Collider >();
}
