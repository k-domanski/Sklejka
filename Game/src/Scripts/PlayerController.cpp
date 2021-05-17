#include "PlayerController.h"
#include <Systems/NodeSystem.h>

#include "Components/UIRenderer.h"

using namespace Engine;
using namespace Engine::ECS;
PlayerController::PlayerController(const std::shared_ptr< Engine::Transform >& player_transform)
    : _playerTransform(player_transform) {
}

auto PlayerController::OnCreate() -> void {
  _bar            = std::make_shared< BarData >();
  auto entity     = ECS::EntityManager::GetInstance().CreateEntity();
  auto renderer   = entity->AddComponent< Components::UIRenderer >();
  _bar->transform = entity->AddComponent< Transform >();
  _bar->bar       = std::make_shared< Renderer::Bar >();
  renderer->GetElements().push_back(_bar->bar);

  _bar->bar->FillRatio(0.5f);
  _bar->transform->Position(glm::vec3(400.0f, 200.0f, 0.0f));
  _bar->bar->BackgroundColor(glm::vec4(0.0f, 1.0f, 1.0f, 1.0f));
  _bar->bar->FillColor(glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));
  _bar->bar->Size(glm::vec2(300.0f, 100.0f));
  _bar->bar->Padding(glm::vec2(5.0f, 5.0f));
  _bar->text = std::make_shared< Renderer::Text >();
  _bar->text->Color(glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
  _bar->text->Size(1.0f);
  _bar->text->SetText("Example SHADOW bar");

  renderer->GetElements().push_back(_bar->text);


  _nodeSystem    = ECS::EntityManager::GetInstance().GetSystem< NodeSystem >();
  _currentNode   = _nodeSystem->GetNode(0);
  _nodeTransform = EntityManager::GetComponent< Transform >(_currentNode->GetEntityID());
  time           = 0.0f;
}

auto PlayerController::Update(float deltaTime) -> void {
  // auto new_pos = _playerTransform->Position() + _playerTransform->Forward() * _speed * deltaTime;
  /* Replace it with Transform Right/Up/Forward once model gets the correct pivot */

  time += deltaTime;

  _bar->bar->FillRatio(sin(time));
  if (time > 360.0f)
    time -= 360.0f;

  /* Seek target */
  SeekTarget(deltaTime);

  auto vertical_move =
      ((float)Engine::Input::IsKeyPressed(Key::W)) - ((float)Engine::Input::IsKeyPressed(Key::S));
  auto horizontal_move =
      ((float)Engine::Input::IsKeyPressed(Key::D)) - ((float)Engine::Input::IsKeyPressed(Key::A));
  auto roll =
      ((float)Engine::Input::IsKeyPressed(Key::E)) - ((float)Engine::Input::IsKeyPressed(Key::Q));

  auto move_delta = glm::vec3{0.0f};
  move_delta += vertical_move * _playerTransform->Up();
  move_delta += horizontal_move * _playerTransform->Right();

  //_playerTransform->Position(_playerTransform->Position() + move_delta * _speed * deltaTime);
  _playerTransform->Position(_playerTransform->Position()
                             + (glm::normalize(_moveVelocity) + move_delta) * _speed * deltaTime);
  _playerTransform->Rotate(roll * deltaTime, _playerTransform->Forward());
}

auto PlayerController::SeekTarget(float deltaTime) -> void {
  auto node = GetNode();
  auto desired_velocity =
      glm::normalize(_nodeTransform->Position() - _playerTransform->Position()) * _speed;
  auto velocity_delta = desired_velocity - _moveVelocity;
  auto new_velocity   = _moveVelocity + velocity_delta * _seekSpeed * deltaTime;
  _moveVelocity       = new_velocity;
  _playerTransform->Forward(glm::normalize(new_velocity));
}

auto PlayerController::GetNode() -> std::shared_ptr< Engine::Node > {
  const auto& node_tr   = EntityManager::GetComponent< Transform >(_currentNode->GetEntityID());
  const auto delta      = node_tr->Position() - _playerTransform->Position();
  const auto magnitude2 = glm::dot(delta, delta);
  if (magnitude2 <= (_minNodeDistance * _minNodeDistance)) {
    _currentNode   = _nodeSystem->GetNode(_currentNode->NextIndex());
    _nodeTransform = EntityManager::GetComponent< Transform >(_currentNode->GetEntityID());
  }
  return _currentNode;
}
