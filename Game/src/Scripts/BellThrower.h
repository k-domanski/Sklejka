#include <Engine.h>

class BellThrower : public Engine::Script<BellThrower>
{
private:
  float _maxDistance;
  float _timeout = 3.f;
  float _currentTimeout = 0.0f;
  std::vector< std::shared_ptr< Engine::ECS::Entity > > _bells;
  std::shared_ptr< Engine::Transform > _playerTransform;
  std::shared_ptr< Engine::Systems::SceneGraph > _sg;

public:
  auto OnCreate() -> void override;
  auto Update(float deltaTime) -> void override;
  auto OnKeyPressed(Engine::Key key) -> void override;
};
