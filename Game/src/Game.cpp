#include <Engine.h>
#include <App/EntryPoint.h>
#include <GameLayer.h>

class Game : public Engine::Application {
public:
  Game() {
    AddLayer(new GameLayer());
  }
};

Engine::Application* Engine::CreateApplication() {
  return new Game();
}