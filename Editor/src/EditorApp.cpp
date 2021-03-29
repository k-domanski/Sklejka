#include "App/Application.h"
#include "App/EntryPoint.h"

class Editor : public Engine::Application
{
public:
	Editor() {}
	~Editor() {}
};

Engine::Application* Engine::CreateApplication()
{
	return new Editor();
}