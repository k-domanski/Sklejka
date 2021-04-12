#pragma once
#include "Window.h"
#include <App/LayerStack.h>
#include "Timer.h"
#include "ECS/System.h"

namespace Engine {
	class Application
	{
	public:
		Application();
		virtual ~Application();
		void Run();
		void AddLayer(Layer* layer);
		void OnEvent(Event& event);
	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
	private:
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		LayerStack m_LayerStack;
		Timer timer;

		//ECS
		std::shared_ptr<ECS::System> m_RendererSystem;
	};

	Application* CreateApplication();
}
