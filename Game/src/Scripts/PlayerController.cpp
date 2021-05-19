#include "PlayerController.h"
#include <Systems/NodeSystem.h>

#include "Components/UIRenderer.h"

using namespace Engine;
using namespace Engine::ECS;
PlayerController::PlayerController(const std::shared_ptr< Engine::Transform >& player_transform)
    : _transform(player_transform) {
}

auto PlayerController::OnCreate() -> void {
  /*_bar            = std::make_shared< BarData >();
  auto entity     = ECS::EntityManager::GetInstance().CreateEntity();
  auto renderer   = entity->AddComponent< Components::UIRenderer >();
  _bar->transform = entity->AddComponent< Engine::Transform >();
  _bar->bar       = std::make_shared< Renderer::Bar >();
  renderer->GetElements().push_back(_bar->bar);*/

  /*_bar->bar->FillRatio(0.5f);
  _bar->transform->Position(glm::vec3(400.0f, 200.0f, 0.0f));
  _bar->bar->BackgroundColor(glm::vec4(0.0f, 1.0f, 1.0f, 1.0f));
  _bar->bar->FillColor(glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));
  _bar->bar->Size(glm::vec2(300.0f, 100.0f));
  _bar->bar->Padding(glm::vec2(5.0f, 5.0f));*/
  /*_bar->text = std::make_shared< Renderer::Text >();
  _bar->text->Color(glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
  _bar->text->Size(1.0f);
  _bar->text->SetText("Example SHADOW bar");

  renderer->GetElements().push_back(_bar->text);*/
}

auto PlayerController::Update(float deltaTime) -> void {
}

auto PlayerController::Transform() const noexcept -> std::shared_ptr< Engine::Transform > {
  return _transform;
}
