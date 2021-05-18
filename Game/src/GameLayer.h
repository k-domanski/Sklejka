#pragma once
#include <Engine.h>

class GameLayer : public Engine::Layer {
private:
  std::shared_ptr< Engine::ECS::Entity > _player = nullptr;

public:
  GameLayer();
  auto OnAttach() -> void override;
  auto OnDetach() -> void override;
  auto OnUpdate(double deltaTime) -> void override;
  auto OnEvent(Engine::Event& event) -> void override;

private:
  auto SetupPlayer(std::shared_ptr< Engine::ECS::Entity >& player) -> void;
};