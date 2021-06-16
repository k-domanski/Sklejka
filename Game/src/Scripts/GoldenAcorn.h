#pragma once
#include "Engine.h"
#include "Settings/PlayerSettings.h"
#include "PlayerRect.h"

namespace Engine {
  namespace Components {
    class UIRenderer;
  }
}  // namespace Engine

class GoldenAcorn : public Engine::Script< GoldenAcorn > {
private:
  std::shared_ptr< Engine::Transform > _transform;
  glm::vec3 _startLocalPos;
  float _wobbleAnimAmplitude = .005f;
  float _wobbleAnimInterval = 2.5f;

  float _throwTime = .5f;
  float _throwSpeed = .1f;

  bool _wobbleAnim = true;
  bool _throw = false;
  
public:
  GoldenAcorn();
  auto OnCreate() -> void override;
  auto Update(float deltaTime) -> void override;
  auto OnKeyPressed(Engine::Key key) -> void override;
  auto OnBossKilled() -> void;
  auto ResetLocalPos() -> void;
  auto DetachFromBoss() -> void;
};