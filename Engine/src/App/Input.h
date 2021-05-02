#pragma once

namespace Engine {
  class Input {
  public:
    static bool IsKeyPressed(int keycode);
    static bool IsMouseButtonPressed(int keycode);

    static glm::vec2 GetMousePosition();
    static float GetMouseX();
    static float GetMouseY();

    /*Gamepad*/
    static bool IsGamepadButtonPressed(int keycode);
    static float GetGamepadAxis(int axis);
  };
}  // namespace Engine