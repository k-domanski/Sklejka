#pragma once
#include <Engine.h>

class GameLayer : public Engine::Layer {
public:
  GameLayer();
  auto OnAttach() -> void override;
  auto OnDetach() -> void override;
  auto OnUpdate(double deltaTime) -> void override;
  auto OnEvent(Engine::Event& event) -> void override;
};