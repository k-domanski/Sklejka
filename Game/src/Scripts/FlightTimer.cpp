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

  _button = std::make_shared< Engine::Renderer::Button >();
  _button->Size(glm::vec2(150.0f, 50.0f));
  _button->TextOffset(glm::vec2(-80.0f, -20.0f));
  _button->PressedColor(glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));
  _button->Color(glm::vec4(0.0f, 1.0f, 1.0f, 1.0f));
  _button->TextColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
  _button->HandleSize(glm::vec2(150.0f, 50.0f));
  //_button->text().Size(1);
  _button->text("Reset time");

  _button->OnPress([this]() { this->ResetTimer(); });

  renderer->GetElements().push_back(_text);
  renderer->GetElements().push_back(_button);
  renderer->GetButtons().push_back(_button);
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
