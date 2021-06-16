#include "pch.h"
#include "Application.h"
#include "GL/GLCore.h"
#include "ECS/EntityManager.h"
#include "Systems/Renderer.h"

namespace Engine {
  Application::Application() {
    /* Fullscreen */
    m_Window = std::unique_ptr< Window >(
        Window::Create(WindowProperties(1600, 900, "Squirrel Ninja", true)));
    //m_Window = std::unique_ptr< Window >(
    //    Window::Create(WindowProperties(1600, 900, "Squirrel Ninja", false)));

    m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));

    m_ImGuiLayer = new ImGuiLayer();
    AddOverlay(m_ImGuiLayer);

    GL::Context::Initialize();
    GL::Context::SetClearBufferMask(GL::BufferBit::Color | GL::BufferBit::Depth
                                    | GL::BufferBit::Stencil);
    GL::Context::SetClearColor({0.1f, 0.1f, 0.1f, 1.0f});
  }
  Application::~Application() {
  }

  void Application::Run() {
    while (m_Running) {
      timer.Update();
      GL::Context::ClearBuffers();
      if (!m_Minimized) {

        for (Layer* layer : m_LayerStack)
          layer->OnUpdate(timer.DeltaTime());

        /*ImGui Render*/
        m_ImGuiLayer->Begin();
        for (Layer* layer : m_LayerStack)
          layer->OnImGuiRender();
        m_ImGuiLayer->End();
      }
      m_Window->OnUpdate();
    }
  }
  void Application::AddLayer(Layer* layer) {
    layer->OnAttach();
    m_LayerStack.AddLayer(layer);
  }
  void Application::AddOverlay(Layer* layer) {
    layer->OnAttach();
    m_LayerStack.AddOverlay(layer);
  }
  void Application::OnEvent(Event& event) {
    EventDispatcher dispatcher(event);
    dispatcher.Dispatch< WindowCloseEvent >(BIND_EVENT_FN(Application::OnWindowClose));
    dispatcher.Dispatch< WindowResizeEvent >(BIND_EVENT_FN(Application::OnWindowResize));

    for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it) {
      if (event.Handled) {
        break;
      }
      (*it)->OnEvent(event);
    }
  }
  bool Application::OnWindowClose(WindowCloseEvent& e) {
    m_Running = false;
    return true;
  }
  bool Application::OnWindowResize(WindowResizeEvent& e) {
      if (e.GetWidth() == 0 || e.GetHeight() == 0) {
      m_Minimized = true;
        return true;
    }

    m_Minimized = false;
    m_Window->SetScreenSize({e.GetWidth(), e.GetHeight()});
    GL::Context::Viewport(0, 0, e.GetWidth(), e.GetHeight());
    return false;
  }
}  // namespace Engine