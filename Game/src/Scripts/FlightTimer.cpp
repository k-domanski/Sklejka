#include "FlightTimer.h"

#include <iomanip>

#include "Engine.h"
#include "GameManager.h"
#include "Components/UIRenderer.h"

FlightTimer::FlightTimer(): Script(), _time(0.0f), _precision(2) {
  auto entity   = Engine::ECS::EntityManager::GetInstance().CreateEntity();
  auto renderer = entity->AddComponent< Engine::Components::UIRenderer >();
  _text         = std::make_shared< Engine::Renderer::Text >();
  _text->Offset(glm::vec2(600.0f, 0.0f));
  _textTransform = entity->AddComponent< Engine::Transform >();

  renderer->GetElements().push_back(_text);
}

auto FlightTimer::OnCreate() -> void {
  _textTransform->Position(glm::vec3(200.0f, 800.0f, 0.0f));
  _text->SetText("0:00");
  _text->Size(1);
}

auto FlightTimer::Update(float deltaTime) -> void {
  if (_canCount) {
    _time += deltaTime * GameManager::GetGameSettings()->PlayerTimeScale();

    _stream << std::fixed << std::setprecision(_precision) << _time;
    _text->SetText(_stream.str());
    _stream.str(std::string());
  }
}

auto FlightTimer::OnKeyPressed(Engine::Key key) -> void{
}

auto FlightTimer::CanCount() -> bool {
  return _canCount;
}

auto FlightTimer::CanCount(bool value) -> void {
  _canCount = value;
}

auto FlightTimer::ResetTimer() -> void {
  _time = 0;
}

auto FlightTimer::GetTime() -> float {
  return _time;
}
