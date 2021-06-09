#pragma once
#include <Engine.h>

class GameLayer : public Engine::Layer {
private:

  std::shared_ptr< Engine::ECS::Entity > _model      = nullptr;

public:
  GameLayer();
  auto OnAttach() -> void override;
  auto OnDetach() -> void override;
  auto OnUpdate(double deltaTime) -> void override;
  auto OnEvent(Engine::Event& event) -> void override;
  bool OnMouseButtonPress(Engine::MouseButtonPressedEvent& e);
  bool OnMouseButtonRelease(Engine::MouseButtonReleasedEvent& e);
  bool OnKeyPress(Engine::KeyPressedEvent& e);

private:
};