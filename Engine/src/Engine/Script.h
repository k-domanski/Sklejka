#pragma once
#include <Engine/IScript.h>
#include <memory>
#include <Components/Collider.h>

namespace Engine {
  class Script : public IScript {
    friend class NativeScript;

  private:
    int _priority{0};
    std::shared_ptr< ECS::Entity > _entity{nullptr};

  public:
    Script() = default;
    Script(int priority): _priority(priority) {
    }
    ~Script() = default;
    auto OnCreate() -> void override {
    }
    auto Update(float deltaTime) -> void override {
    }
    auto OnDestroy() -> void override {
    }
    auto OnCollisionEnter(const std::shared_ptr< Components::Collider >& collider) -> void {
    }
    auto Priority() -> int override {
      return _priority;
    }
    auto Entity() const -> std::shared_ptr< ECS::Entity > {
      return _entity;
    }

  private:
    auto Entity(const std::shared_ptr< ECS::Entity >& entity) -> void {
      _entity = entity;
    }
  };
}  // namespace Engine
