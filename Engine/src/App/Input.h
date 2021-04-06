#pragma once

namespace Engine {
  class Input {
  public:
    static bool IsKeyPressed(int keycode);
    static bool IsMouseButtonPressed(int keycode);

    static glm::vec2 GetMousePosition();
    static float GetMouseX();
    static float GetMouseY();
  };
}  // namespace Engine