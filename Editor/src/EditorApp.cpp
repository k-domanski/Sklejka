#include "Engine.h"
#include "App/EntryPoint.h"
#include "EditorLayer.h"
#include "ImGui/ImGui/ImGuiLayer.h"

class EditorApplication : public Engine::Application {
public:
  EditorApplication() {
    AddLayer(new EditorLayer("Test"));
    //AddOverlay(new ImGuiLayer());
  }
  ~EditorApplication() {
  }
};

Engine::Application* Engine::CreateApplication() {
  return new EditorApplication();
}