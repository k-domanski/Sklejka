#pragma once
#include <Engine.h>
#include "Utility/Utility.h"

class CutscenePlayer : public Engine::Script<CutscenePlayer> {
  typedef Engine::Utility::TimeLerp< glm::vec2 > Vec2Lerp;

private:
  std::shared_ptr< Engine::GL::Shader > _shader;
  Vec2Lerp _tilingLerp;
  Vec2Lerp _offsetLerp;
  std::vector< std::array< glm::vec2, 2 > > _transitions;
  int _phase = 0;
  float _duration;
  float _stopTime;
  float _time;
  float _transitionTime;

public:
  CutscenePlayer(const std::shared_ptr< Engine::GL::Shader >& shader);
  auto OnCreate() -> void override;
  auto Update(float deltaTime) -> void override;
  auto OnKeyPressed(Engine::Key key) -> void override;
  auto Reset() -> void;

private:
  auto TilingOffset(const glm::vec2 tiling, const glm::vec2 offset) -> void;
  auto NextPhase() -> void;
};