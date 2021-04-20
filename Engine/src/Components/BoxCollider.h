#pragma once
#include "ECS/Component.h"

namespace Engine::Components {
  class BoxCollider : public ECS::Component {
  public:
    BoxCollider(): Component("Box Collider") {
    }
    BoxCollider(std::string filePath);
    ~BoxCollider() override = default;
    auto GetSize() -> glm::vec3;
    auto SetSize(glm::vec3 size) -> void;
    auto IsTrigger() -> bool;
    auto SetTrigger(bool trigger) -> void;
    std::string SaveToJson(std::string filePath) override;

  private:
    glm::vec3 _size{0.0f, 0.0f, 0.0f};
    bool _trigger = false;
  };
}  // namespace Engine::Components
