#pragma once
#include <Engine.h>
#include "Components/Transform.h"
#include "Engine/Script.h"
#include "Renderer/Button.h"
#include "Renderer/Text.h"

class FlightTimer : public Engine::Script< FlightTimer > {
public:
  FlightTimer();
  auto OnCreate() -> void override;
  auto Update(float deltaTime) -> void override;
  auto OnKeyPressed(Engine::Key key) -> void override;
  auto CanCount() -> bool;
  auto CanCount(bool value) -> void;
  auto ResetTimer() -> void;
  auto GetTime() -> float;

private:
  bool _canCount;
  float _time;
  std::shared_ptr< Engine::Renderer::Text > _text;
  std::shared_ptr< Engine::Renderer::Image > _clockImage;
  std::shared_ptr< Engine::Transform > _textTransform;
  std::stringstream _stream;
  int _precision;
};
