#include "StartTimer.h"

#include <iomanip>

#include "Engine.h"
#include "Components/UIRenderer.h"

StartTimer::StartTimer(): Script(), _time(0.0f), _precision(0) {
  auto entity    = Engine::ECS::EntityManager::GetInstance().CreateEntity();
  auto renderer  = entity->AddComponent< Engine::Components::UIRenderer >();
  _text          = std::make_shared< Engine::Renderer::Text >();
  _textTransform = entity->AddComponent< Engine::Transform >();
  renderer->GetElements().push_back(_text);
}

auto StartTimer::OnCreate() -> void {
  _textTransform->Position(glm::vec3(800.0f, 1200.0f, 0.0f));
  _text->SetText("0");
  _text->Size(1);
}

auto StartTimer::Update(float deltaTime) -> void {
  _time += deltaTime;

  _stream << std::fixed << std::setprecision(_precision) << _time;
  _text->SetText(_stream.str());
  _stream.str(std::string());
}
