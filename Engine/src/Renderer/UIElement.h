#pragma once

namespace Engine::Renderer {
  class UIElement {
  public:
    UIElement()                                                = default;
    virtual ~UIElement()                                       = default;
    auto virtual Draw(glm::mat4 model, glm::mat4 proj) -> void = 0;
  };

}  // namespace Engine::Renderer
