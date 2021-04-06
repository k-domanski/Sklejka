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
}  // namespace Engine