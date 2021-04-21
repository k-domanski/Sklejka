#include "Engine.h"
#include "App/EntryPoint.h"
#include "EditorLayer.h"

class EditorApplication : public Engine::Application {
public:
  EditorApplication() {
    AddLayer(new EditorLayer("Test"));
  }
  ~EditorApplication() {
  }
};

Engine::Application* Engine::CreateApplication() {
  return new EditorApplication();
}