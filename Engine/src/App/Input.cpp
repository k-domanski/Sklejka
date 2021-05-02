#include "pch.h"
#include "Input.h"
#include "Window.h"

namespace Engine {
  bool Input::IsKeyPressed(int keycode) {
    auto* window = static_cast< GLFWwindow* >(Window::Get().GetNativeWindow());
    auto state   = glfwGetKey(window, keycode);
    return state == GLFW_PRESS || state == GLFW_REPEAT;
  }

  bool Input::IsMouseButtonPressed(int keycode) {
    auto* window = static_cast< GLFWwindow* >(Window::Get().GetNativeWindow());
    auto state   = glfwGetMouseButton(window, keycode);
    return state == GLFW_PRESS;
  }

  glm::vec2 Input::GetMousePosition() {
    auto* window = static_cast< GLFWwindow* >(Window::Get().GetNativeWindow());
    double xPos, yPos;
    glfwGetCursorPos(window, &xPos, &yPos);
    return {(float)xPos, (float)yPos};
  }

  float Input::GetMouseX() {
    return GetMousePosition().x;
  }

  float Input::GetMouseY() {
    return GetMousePosition().y;
  }

  bool Input::IsGamepadButtonPressed(int keycode) {
    if (!glfwJoystickPresent(GLFW_JOYSTICK_1))
      return false;

    int count;
    const unsigned char* buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &count);
    if (buttons[keycode]) {
      return true;
    }
    return false;
  }

  float Input::GetGamepadAxis(int axis) {
    if (!glfwJoystickPresent(GLFW_JOYSTICK_1))
      return false;
    int count;
    const float* axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &count);
    return axes[axis];
  }
}  // namespace Engine