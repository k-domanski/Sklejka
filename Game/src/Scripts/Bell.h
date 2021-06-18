#include <Engine.h>
#include <Utility/Utility.h>

class Bell : public Engine::Script< Bell > {
private:
  float _timeout;
  float _remainingTimeout;
  float _turnTime;
  glm::quat _baseRotation;
  glm::quat _turnRotation;
  std::shared_ptr< Engine::Transform > _transform;
  Engine::Utility::QuatLerp _quatLerp;

public:
  auto OnCreate() -> void override;
  auto Update(float deltaTime) -> void override;
  auto OnCollisionEnter(const std::shared_ptr< Engine::Components::Collider >& collider)
      -> void override;
  auto IsActive() -> bool;

private:
  auto TurnBell(glm::quat start, glm::quat end, float timeout) -> void;
};