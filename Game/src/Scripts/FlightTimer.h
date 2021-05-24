#pragma once
#include <Engine.h>
#include "Components/Transform.h"
#include "Engine/Script.h"
#include "Renderer/Button.h"
#include "Renderer/Text.h"

class FlightTimer : public Engine::Script {
public:
  FlightTimer();
  auto OnCreate() -> void override;
  auto Update(float deltaTime) -> void override;
  auto ResetTimer() -> void;

private:
  float _time;
  std::shared_ptr< Engine::Renderer::Text > _text;
  std::shared_ptr< Engine::Transform > _textTransform;
  std::stringstream _stream;
  int _precision;
  std::shared_ptr<Engine::Renderer::Button> _button;
};
