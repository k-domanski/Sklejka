#include "pch.h"
#include "Application.h"
#include "GL/GLCore.h"


namespace Engine {
	Application::Application() {
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));

		GL::Context::Initialize();
		GL::Context::SetClearBufferMask(GL::BufferBit::Color);
		GL::Context::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	}
	Application::~Application() {

	}

	void Application::Run()
	{
		while (m_Running)
		{
			timer.Update();
			GL::Context::ClearBuffers();
			
			for (Layer* layer : m_LayerStack)
				layer->OnUpdate(timer.DeltaTime());

			m_Window->OnUpdate();
		}
	}
	void Application::AddLayer(Layer* layer)
	{
		layer->OnAttach();
		m_LayerStack.AddLayer(layer);
	}
	void Application::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		//CORE_DEBUG("{0}", event);
		dispatcher.Dispatch<WindowCloseEvent>([this](WindowCloseEvent& e)
			{
				m_Running = false;
				return true;
			});
	}
}