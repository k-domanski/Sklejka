#include "FlightTimer.h"

#include <iomanip>

#include "Engine.h"
#include "Components/UIRenderer.h"

FlightTimer::FlightTimer(): Script(), _time(0.0f), _precision(2) {
  auto entity    = Engine::ECS::EntityManager::GetInstance().CreateEntity();
  auto renderer  = entity->AddComponent< Engine::Components::UIRenderer >();
  _text          = std::make_shared< Engine::Renderer::Text >();
  _textTransform = entity->AddComponent< Engine::Transform >();
  renderer->GetElements().push_back(_text);
}

auto FlightTimer::OnCreate() -> void {
  _textTransform->Position(glm::vec3(800.0f, 800.0f, 0.0f));
  _text->SetText("0:00");
  _text->Size(1);
}

auto FlightTimer::Update(float deltaTime) -> void {
  _time += deltaTime;
  
  _stream << std::fixed << std::setprecision(_precision) << _time;
  _text->SetText(_stream.str());
  _stream.str(std::string());
}
