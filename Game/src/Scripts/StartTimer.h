#pragma once
#include <Engine.h>
#include "Components/Transform.h"
#include "Engine/Script.h"
#include "Renderer/Text.h"

class StartTimer : public Engine::Script {
public:
  StartTimer();
  auto OnCreate() -> void override;
  auto Update(float deltaTime) -> void override;

private:
  float _time;
  std::shared_ptr< Engine::Renderer::Text > _text;
  std::shared_ptr< Engine::Transform > _textTransform;
  std::stringstream _stream;
  int _precision;
};
