#pragma once
#include <Engine.h>
#include "Components/Transform.h"
#include "Engine/Script.h"
#include "Renderer/Text.h"

class FlightTimer;
class PlayerRect;

class StartTimer : public Engine::Script<StartTimer> {
public:
  StartTimer(std::shared_ptr< PlayerRect > playerRect, std::shared_ptr< FlightTimer > flightTimer, std::shared_ptr<ShadowTarget> shadowTarget);
  auto OnCreate() -> void override;
  auto Update(float deltaTime) -> void override;
  auto OnKeyPressed(Engine::Key key) -> void override;
  auto CanCount() -> bool;
  auto CanCount(bool value) -> void;

private:
  bool _canCount;
  float _time;
  std::shared_ptr< PlayerRect > _playerRect;
  std::shared_ptr< FlightTimer> _flightTimer;
  std::shared_ptr< ShadowTarget> _shadowTarget;
  std::shared_ptr< Engine::Renderer::Text > _text;
  std::shared_ptr< Engine::Transform > _textTransform;
  std::stringstream _stream;
  int _precision;
};
