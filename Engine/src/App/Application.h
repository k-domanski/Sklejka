#pragma once
#include "Window.h"
#include "App/LayerStack.h"
#include "Timer.h"
#include "ECS/System.h"

namespace Engine {
  class Application {
  public:
    Application();
    virtual ~Application();
    void Run();
    void AddLayer(Layer* layer);
    void AddOverlay(Layer* layer);
    void OnEvent(Event& event);
    static Application& Get() {
      return *s_Instance;
    }
  private:
    bool OnWindowClose(WindowCloseEvent& e);
    bool OnWindowResize(WindowResizeEvent& e);

  private:
    static Application* s_Instance;
    std::unique_ptr< Window > m_Window;
    bool m_Running = true;
    bool m_Minimized = false;
    LayerStack m_LayerStack;
    Timer timer;
  };

  Application* CreateApplication();
}  // namespace Engine
