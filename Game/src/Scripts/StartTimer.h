#pragma once
#include <Engine.h>
#include "Components/Transform.h"
#include "Engine/Script.h"
#include "Renderer/Text.h"

class FlightTimer;
class PlayerRect;

class StartTimer : public Engine::Script {
public:
  StartTimer(std::shared_ptr< PlayerRect > playerRect, std::shared_ptr< FlightTimer > flightTimer);
  auto OnCreate() -> void override;
  auto Update(float deltaTime) -> void override;
  auto CanCount() -> bool;
  auto CanCount(bool value) -> void;

private:
  bool _canCount;
  float _time;
  std::shared_ptr< PlayerRect > _playerRect;
  std::shared_ptr< FlightTimer> _flightTimer;
  std::shared_ptr< Engine::Renderer::Text > _text;
  std::shared_ptr< Engine::Transform > _textTransform;
  std::stringstream _stream;
  int _precision;
};
