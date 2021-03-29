#include "pch.h"
#include "Application.h"

namespace Engine {
	Application::Application() {
		m_Window = std::unique_ptr<Window>(Window::Create());
	}
	Application::~Application() {

	}

	void Application::Run()
	{
		while (m_Running)
		{
			m_Running = !glfwWindowShouldClose(m_Window->GetNativeWindow());
			glClearColor(1, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			m_Window->OnUpdate();
		}
	}
}