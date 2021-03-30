#pragma once
#include <string>
#include <memory>
#include "GLFW/glfw3.h"

namespace Engine {
  struct WindowData {
    std::string Name;
    int Width;
    int Height;

    WindowData(int width = 1280, int height = 720, const std::string& name = "Engine")
        : Width(width), Height(height), Name(name) {
    }
  };

  class Window {
  public:
    Window(const WindowData& data);
    ~Window();
    void OnUpdate();
    int GetWidth() const { return m_Data.Width; }
    int GetHeight() const { return m_Data.Height; }
    GLFWwindow* GetNativeWindow() const { return m_Window; }
    static std::unique_ptr< Window > Create(const WindowData& data = WindowData());

  private:
    void Init(const WindowData& data);
    void ShutDown();

  private:
    GLFWwindow* m_Window;
    WindowData m_Data;
  };
}  // namespace Engine
