#pragma once
#include <Engine.h>

class PlayerController : public Engine::Script {
private:
  std::shared_ptr< Engine::Transform > _transform;

public:
  PlayerController(const std::shared_ptr< Engine::Transform >& player_transform);
  auto OnCreate() -> void override;
  auto Update(float deltaTime) -> void override;
  auto OnKeyPressed(Engine::Key key) -> void override;
  auto Transform() const noexcept -> std::shared_ptr< Engine::Transform >;
};