#pragma once
#include "Window.h"
#include <App/LayerStack.h>

namespace Engine {
	class Application
	{
	public:
		Application();
		virtual ~Application();
		void Run();
		void AddLayer(Layer* layer);
	private:
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		LayerStack m_LayerStack;
	};

	Application* CreateApplication();
}
