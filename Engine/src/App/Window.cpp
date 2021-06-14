#include "pch.h"
#include "Window.h"

#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"

namespace Engine {
  Window* Window::s_Instance = nullptr;

  static void GlfwErrorCallback(int error, const char* description) {
    CORE_ERROR("GLFW Error ({0}): {1}", error, description);
  }
  Window::Window(const WindowProperties& data) {
    if (s_Instance != nullptr) {
      CORE_INFO("Window already exists.");
    }

    s_Instance = this;

    Init(data);
  }
  Window::~Window() {
    ShutDown();
  }
  void Window::OnUpdate() {
    glfwPollEvents();
    glfwSwapBuffers(m_Window);
  }
  std::unique_ptr< Window > Window::Create(const WindowProperties& data) {
    return std::make_unique< Window >(data);
  }
  void Window::Init(const WindowProperties& data) {
    bool init = glfwInit();
    if (!init)
      CORE_ERROR("Could not initialize GLFW.");
    glfwSetErrorCallback(GlfwErrorCallback);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    if (data.Fullscreen) {
      auto monitor            = glfwGetPrimaryMonitor();
      const GLFWvidmode* mode = glfwGetVideoMode(monitor);
      glfwWindowHint(GLFW_RED_BITS, mode->redBits);
      glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
      glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
      glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

      m_Window      = glfwCreateWindow(mode->width, mode->height, data.Name.c_str(), monitor, NULL);
      m_Data.Width  = mode->width;
      m_Data.Height = mode->height;
    } else {
      m_Window      = glfwCreateWindow(data.Width, data.Height, data.Name.c_str(), NULL, NULL);
      m_Data.Width  = data.Width;
      m_Data.Height = data.Height;
    }
    m_Data.Name = data.Name;

    if (!m_Window) {
      CORE_ERROR("Window not created.");
      glfwTerminate();
    }

    glfwMakeContextCurrent(m_Window);
    glfwSetWindowUserPointer(m_Window, &m_Data);
    glfwSwapInterval(0);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
      CORE_ERROR("Could not load glad.");
    }

    // GLFW callbacks
    glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
      WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
      data.Width       = width;
      data.Height      = height;

      WindowResizeEvent event(width, height);
      data.EventCallback(event);
    });

    glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
      WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
      WindowCloseEvent event;
      data.EventCallback(event);
    });

    glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods) {
      WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
      switch (action) {
        case GLFW_PRESS: {
          MouseButtonPressedEvent event(button);
          data.EventCallback(event);
          break;
        }
        case GLFW_RELEASE: {
          MouseButtonReleasedEvent event(button);
          data.EventCallback(event);
          break;
        }
      }
    });

    glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos) {
      WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
      MouseMovedEvent event((float)xPos, (float)yPos);
      data.EventCallback(event);
    });

    glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset) {
      WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
      MouseScrolledEvent event((float)xOffset, (float)yOffset);
      data.EventCallback(event);
    });

    glfwSetKeyCallback(m_Window,
                       [](GLFWwindow* window, int key, int scancode, int action, int mods) {
                         WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
                         switch (action) {
                           case GLFW_PRESS: {
                             KeyPressedEvent event(key, 0);
                             data.EventCallback(event);
                             break;
                           }
                           case GLFW_RELEASE: {
                             KeyReleasedEvent event(key);
                             data.EventCallback(event);
                             break;
                           }
                           case GLFW_REPEAT: {
                             KeyPressedEvent event(key, 1);
                             data.EventCallback(event);
                             break;
                           }
                         }
                       });
  }
  void Window::ShutDown() {
    glfwDestroyWindow(m_Window);
    glfwTerminate();
  }
}  // namespace Engine