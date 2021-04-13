#include "pch.h"
#include "Application.h"
#include "GL/GLCore.h"
#include "ECS/EntityManager.h"
#include "Systems/Renderer.h"

namespace Engine {
  Application::Application() {
    m_Window = std::unique_ptr< Window >(Window::Create());
    m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));

    GL::Context::Initialize();
    GL::Context::SetClearBufferMask(GL::BufferBit::Color);
    GL::Context::SetClearColor({0.1f, 0.1f, 0.1f, 1.0f});
  }
  Application::~Application() {
  }

  void Application::Run() {
    while (m_Running) {
      timer.Update();
      GL::Context::ClearBuffers();

      for (Layer* layer : m_LayerStack)
        layer->OnUpdate(timer.DeltaTime());

      /*Na razie tutaj update systemow*/
      ECS::EntityManager::GetInstance().Update();
      m_Window->OnUpdate();
    }
  }
  void Application::AddLayer(Layer* layer) {
    layer->OnAttach();
    m_LayerStack.AddLayer(layer);
  }
  void Application::OnEvent(Event& event) {
    EventDispatcher dispatcher(event);
    dispatcher.Dispatch< WindowCloseEvent >(BIND_EVENT_FN(Application::OnWindowClose));
    dispatcher.Dispatch< WindowResizeEvent >(BIND_EVENT_FN(Application::OnWindowResize));

    for (auto it = m_LayerStack.begin(); it != m_LayerStack.end(); ++it) {
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
    return false;
  }
}  // namespace Engine