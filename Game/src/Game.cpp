#include <Engine.h>
#include <App/EntryPoint.h>

class Game : public Engine::Application {
public:
  Game() {
  }
};

Engine::Application* Engine::CreateApplication() {
  return new Game();
}