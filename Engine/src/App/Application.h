#pragma once
#include "Window.h"
#include <App/LayerStack.h>
#include "Timer.h"

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
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		LayerStack m_LayerStack;
		Timer timer;
	};

	Application* CreateApplication();
}
