#pragma once

namespace Engine::Renderer {
  class UIElement {
  public:
    UIElement() {
      _isActive = true;
    }
    virtual ~UIElement()                                       = default;
    auto virtual Draw(glm::mat4 model, glm::mat4 proj) -> void = 0;
    auto virtual OnWindowResize(glm::vec2 ratio) -> void = 0;
    auto SetActive(bool active) -> void;
    auto IsActive() -> bool;

  private:
    bool _isActive;
  };

}  // namespace Engine::Renderer
