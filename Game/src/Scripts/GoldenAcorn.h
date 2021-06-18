#pragma once
#include "Engine.h"
#include "Settings/PlayerSettings.h"
#include "PlayerRect.h"
#include "Utility/Utility.h"

namespace Engine {
  namespace Components {
    class UIRenderer;
  }
}  // namespace Engine

using namespace Engine;
class GoldenAcorn : public Engine::Script< GoldenAcorn > {
private:
  std::shared_ptr< Engine::Transform > _transform;
  glm::vec3 _startLocalPos;
  float _wobbleAnimAmplitude = .005f;
  float _wobbleAnimInterval  = 2.5f;

  float _throwTime  = .65f;
  float _throwSpeed = .1f;

  bool _wobbleAnim = true;
  bool _throw      = false;

  std::shared_ptr< Transform > _bossTr;
  std::shared_ptr< Transform > _weaselTr;
  float _swapTime;
  bool _animate{false};
  Utility::FloatLerp _animationLerp;
  glm::vec3 _offset{0.0f, 1.0f, 0.0f};

public:
  GoldenAcorn();
  auto OnCreate() -> void override;
  auto Update(float deltaTime) -> void override;
  auto OnKeyPressed(Engine::Key key) -> void override;
  auto OnBossKilled() -> void;
  auto StartTransition(std::shared_ptr< Transform > bossTr, std::shared_ptr< Transform > weaselTr,
                       float swapTime) -> void;
  auto ResetLocalPos() -> void;
  auto DetachFromBoss() -> void;
};