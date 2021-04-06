#pragma once
#include <string>
#include <memory>
#include "GLFW/glfw3.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"

namespace Engine {
  
    struct WindowProperties {
        std::string Name;
        int Width;
        int Height;

        WindowProperties(int width = 1280, int height = 720, const std::string& name = "Engine")
            : Width(width), Height(height), Name(name) {
        }
    };

  class Window {
  public:
      using EventCallBackFn = std::function<void(Event&)>;

    Window(const WindowProperties& data);
    ~Window();
    static Window& Get() { return *s_Instance; }
    void OnUpdate();
    int GetWidth() const { return m_Data.Width; }
    int GetHeight() const { return m_Data.Height; }
    void SetEventCallback(const EventCallBackFn& callback) { m_Data.EventCallback = callback; }
    GLFWwindow* GetNativeWindow() const { return m_Window; }
    static std::unique_ptr< Window > Create(const WindowProperties& data = WindowProperties());

  private:
    void Init(const WindowProperties& data);
    void ShutDown();

  private:
     static Window* s_Instance;
    GLFWwindow* m_Window;
    struct WindowData
    {
        std::string Name;
        int Width;
        int Height;

        EventCallBackFn EventCallback;
    };
    WindowData m_Data;
  };
}  // namespace Engine
