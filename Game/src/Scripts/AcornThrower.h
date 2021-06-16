#include <Engine.h>

class AcornThrower : public Engine::Script< AcornThrower > {
private:
  float _maxDistance;
  float _timeout;
  float _currentTimeout = 0.0f;
  std::vector< std::shared_ptr< Engine::ECS::Entity > > _bells;
  std::shared_ptr< Engine::ECS::Entity > _boss;
  std::shared_ptr< Engine::Transform > _playerTransform;
  std::shared_ptr< Engine::Transform > _bossTransform;
  std::shared_ptr< Engine::Systems::SceneGraph > _sg;
  std::shared_ptr< Engine::Transform > _transform;
  std::shared_ptr< Engine::Renderer::Bar > _bar;
  std::shared_ptr< Engine::Renderer::Image > _image;
  bool _lastStateA;
  auto TryThrow() -> void;
  auto Throw(std::shared_ptr< Engine::Transform > target) -> void;
  auto Throw(glm::vec3 direction) -> void;

public:
  auto OnCreate() -> void override;
  auto Update(float deltaTime) -> void override;
  auto OnKeyPressed(Engine::Key key) -> void override;
};
