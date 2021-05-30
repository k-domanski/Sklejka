#include "CutscenePlayer.h"

CutscenePlayer::CutscenePlayer(const std::shared_ptr< Engine::GL::Shader >& shader)
    : _shader(shader), _duration(0), _stopTime(0), _time(0), _transitionTime(0) {
}

auto CutscenePlayer::OnCreate() -> void {
  /*
    Transitions:
    full -> 1 -> 2 -> 3 -> 4 -> 5 -> full
    1s      1s   1s   1s   1s   1s   1s
    5 panels + 2 full
  */
  _duration = 20.0f;
  _stopTime = 2.0f;

  using v2     = glm::vec2;
  _transitions = {
      {v2{1.0f, 1.0f}, v2{0.0f, 0.0f}},     /* Start */
      {v2{0.45f, 0.45f}, v2{0.0f, 0.51f}},  /* 1st */
      {v2{0.45f, 0.45f}, v2{0.27f, 0.51f}}, /* 2nd */
      {v2{0.45f, 0.45f}, v2{0.52f, 0.51f}}, /* 3rd */
      {v2{0.45f, 0.45f}, v2{0.05f, 0.05f}}, /* 4th */
      {v2{0.45f, 0.45f}, v2{0.49f, 0.05f}}, /* 5th */
      {v2{1.0f, 1.0f}, v2{0.0f, 0.0f}},     /* End */
  };

  auto total_stop_time = _transitions.size() * _stopTime;
  _transitionTime      = (_duration - total_stop_time) / (_transitions.size() - 1);

  _tilingLerp.Set(glm::vec2(1.0f), glm::vec2(1.0f), _transitionTime);
  _offsetLerp.Set(glm::vec2(0.0f), glm::vec2(0.0f), _transitionTime);
}

auto CutscenePlayer::Update(float deltaTime) -> void {
  if (_time > _duration + _stopTime) {
    return;
  }

  auto t = _stopTime * (_phase + 1) + _transitionTime * _phase;
  if (_time > t) {
    NextPhase();
  }

  auto tiling = _tilingLerp.Update(deltaTime);
  auto offset = _offsetLerp.Update(deltaTime);
  TilingOffset(tiling, offset);

  _time += deltaTime;

  if (_time > _duration + _stopTime) {
    LOG_INFO("Finished cutscene");
  }
}

auto CutscenePlayer::OnKeyPressed(Engine::Key key) -> void{
}

auto CutscenePlayer::Reset() -> void {
  _phase = 0;
  _time  = 0.0f;
  TilingOffset({1.0f, 1.0f}, {0.0f, 0.0f});
  _tilingLerp.Set(glm::vec2(1.0f), glm::vec2(1.0f), _transitionTime);
  _offsetLerp.Set(glm::vec2(0.0f), glm::vec2(0.0f), _transitionTime);
}

auto CutscenePlayer::TilingOffset(const glm::vec2 tiling, const glm::vec2 offset) -> void {
  _shader->SetVector("u_Tiling", tiling);
  _shader->SetVector("u_Offset", offset);
}

auto CutscenePlayer::NextPhase() -> void {
  if (_phase < _transitions.size() - 1) {
    ++_phase;
    _tilingLerp.Set(_transitions[_phase - 1][0], _transitions[_phase][0], _transitionTime);
    _offsetLerp.Set(_transitions[_phase - 1][1], _transitions[_phase][1], _transitionTime);
  }
}
