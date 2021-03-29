#include "pch.h"
#include "Window.h"

namespace Engine {
  Window::Window(const WindowData& data) {
    Init(data);
  }
  Window::~Window() {
    ShutDown();
  }
  void Window::OnUpdate() {
    glfwPollEvents();
    glfwSwapBuffers(m_Window);
  }
  std::unique_ptr< Window > Window::Create(const WindowData& data) {
    return std::make_unique< Window >(data);
  }
  void Window::Init(const WindowData& data) {
    m_Data = data;

    if (!glfwInit())
      CORE_ERROR("Could not initialize GLFW.");

    m_Window = glfwCreateWindow(data.Width, data.Height, data.Name.c_str(), NULL, NULL);

    if (!m_Window) {
      CORE_ERROR("Window not created.");
      glfwTerminate();
    }

    glfwMakeContextCurrent(m_Window);
    glfwSwapInterval(1);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
      CORE_ERROR("Could not load glad.");
    }
  }
  void Window::ShutDown() {
    glfwDestroyWindow(m_Window);
    glfwTerminate();
  }
}  // namespace Engine