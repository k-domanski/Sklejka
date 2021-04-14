#pragma once
#include "Window.h"
#include "App/LayerStack.h"
#include "ImGui/ImGuiLayer.h"
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

  private:
    bool OnWindowClose(WindowCloseEvent& e);
    bool OnWindowResize(WindowResizeEvent& e);

  private:
    std::unique_ptr< Window > m_Window;
    bool m_Running = true;
    ImGuiLayer* m_ImGuiLayer;
    LayerStack m_LayerStack;
    Timer timer;
  };

  Application* CreateApplication();
}  // namespace Engine
