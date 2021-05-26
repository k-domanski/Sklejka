#include "StartTimer.h"
#include "FlightTimer.h"
#include "PlayerRect.h"

#include <iomanip>

#include "Engine.h"
#include "Components/UIRenderer.h"

StartTimer::StartTimer(std::shared_ptr< PlayerRect > playerRect,
                       std::shared_ptr< FlightTimer > flightTimer)
    : Script(), _time(0.0f), _precision(0), _playerRect(playerRect), _flightTimer(flightTimer) {
  auto entity    = Engine::ECS::EntityManager::GetInstance().CreateEntity();
  auto renderer  = entity->AddComponent< Engine::Components::UIRenderer >();
  _text          = std::make_shared< Engine::Renderer::Text >();
  _textTransform = entity->AddComponent< Engine::Transform >();
  renderer->GetElements().push_back(_text);
}

auto StartTimer::OnCreate() -> void {
  _textTransform->Position(glm::vec3(800.0f, 400.0f, 0.0f));
  _text->SetText("0");
  _text->Size(2);
  _time = 4.0f;
}

auto StartTimer::Update(float deltaTime) -> void {
  if (_canCount) {
    _time -= deltaTime;

    _stream << std::fixed << std::setprecision(_precision) << _time;
    auto str = std::to_string((int)_time);
    _text->SetText(str);
    _stream.str(std::string());

    if (_time <= 1.0f) {
      _playerRect->CanMove(true);
      _flightTimer->CanCount(true);
      _text->SetText("GO");
      // TODO: Remove timer object
    }
    if (_time <= 0.0f) {
      _text->SetText("");
      // TODO: Remove timer object
      _canCount = false;
    }
  }
}

auto StartTimer::CanCount() -> bool {
  return _canCount;
}

auto StartTimer::CanCount(bool value) -> void {
  _canCount = value;
}
