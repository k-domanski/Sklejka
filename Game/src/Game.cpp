#include <Engine.h>
#include <App/EntryPoint.h>
#include <GameLayer.h>
#include <GameManager.h>

class Game : public Engine::Application {
public:
  Game() {
    GameManager::Initialize();
    AddLayer(new GameLayer());
  }
};

Engine::Application* Engine::CreateApplication() {
  return new Game();
}