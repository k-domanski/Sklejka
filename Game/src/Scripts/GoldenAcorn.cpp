#include "GoldenAcorn.h"

#include "GameManager.h"
#include "Systems/SceneGraph.h"

GoldenAcorn::GoldenAcorn() {
}

auto GoldenAcorn::OnCreate() -> void {
  _transform     = Entity()->GetComponent< Engine::Transform >();
  _startLocalPos = _transform->Position();
}

auto GoldenAcorn::Update(float deltaTime) -> void {
  if (_wobbleAnim) {
    _transform->Position(
        _transform->Position()
        + glm::vec3(0.f, 1.f, 0.f)
              * glm::sin(GameManager::Time() * 2 * glm::pi< float >() / _wobbleAnimInterval)
              * _wobbleAnimAmplitude * GameManager::GetGameSettings()->PlayerTimeScale()
              * GameManager::GetGameSettings()->GameTimeScale());
  }

  if (_animate) {
    if (!_animationLerp.Finished()) {
      auto pos = _bossTr->WorldPosition() + glm::vec3(0.0f, 1.0f, 0.0f)
                 + _offset * _animationLerp.Update(deltaTime);
      _transform->Position(pos);
    } else {
      auto sg = GameManager::GetScene(GameManager::GetCurrentSceneName())->SceneGraph();
      sg->SetParent(Entity(), _weaselTr->GetEntity());
      _transform->Position(glm::vec3(0.0f, -1.5, 0.0f));
      _transform->Scale(glm::vec3(3.0f / 0.4f));
      _wobbleAnim = true;
      _animate    = false;
    }
  }

  /*if (_throw) {
    _transform->Position(_transform->Position() + glm::vec3(0.f, 1.f, 0.f) * _throwSpeed);
    _throwTime -= deltaTime;

    if (_throwTime <= 0.f) {
      _throw = false;
    }
  }*/
}

auto GoldenAcorn::OnKeyPressed(Engine::Key key) -> void {
}

auto GoldenAcorn::OnBossKilled() -> void {
  _wobbleAnim = false;
  _throw      = true;
  DetachFromBoss();
}

auto GoldenAcorn::StartTransition(std::shared_ptr< Transform > bossTr,
                                  std::shared_ptr< Transform > weaselTr, float swapTime) -> void {
  _bossTr     = bossTr;
  _weaselTr   = weaselTr;
  _swapTime   = swapTime;
  _animate    = true;
  _wobbleAnim = false;
  _animationLerp.Set(0.0f, 1.0f, _swapTime);
  DetachFromBoss();
}

auto GoldenAcorn::ResetLocalPos() -> void {
  _transform->Position(_startLocalPos);
}

auto GoldenAcorn::DetachFromBoss() -> void {
  ResetLocalPos();
  glm::vec3 worldPos = _transform->WorldPosition();
  auto sg            = GameManager::GetScene(GameManager::GetCurrentSceneName())->SceneGraph();
  sg->SetParent(Entity(), sg->GetRootEntity());
  _transform->Position(worldPos);
}
