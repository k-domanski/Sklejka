#include "Engine.h"
#include "App/EntryPoint.h"
#include "EditorLayer.h"

class Editor : public Engine::Application {
public:
  Editor() {
    AddLayer(new EditorLayer("Test"));
  }
  ~Editor() {
  }
};

Engine::Application* Engine::CreateApplication() {
  return new Editor();
}