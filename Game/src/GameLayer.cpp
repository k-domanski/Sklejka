#include <GameLayer.h>
#include <filesystem>
#include <Systems/SceneGraph.h>

#include <GameManager.h>
#include "Scripts/StartTimer.h"

using namespace Engine;
GameLayer::GameLayer(): Engine::Layer("Game") {
}

auto GameLayer::OnAttach() -> void {
  LOG_TRACE("Current dir: {}", std::filesystem::current_path().string());
  GameManager::SwitchScene(SceneName::LVL_1);

  auto scene = SceneManager::GetCurrentScene();
  // SetupPlayer(scene);

  // GameManager::ShowLoadingScreen();
}

auto GameLayer::OnDetach() -> void {
}

auto GameLayer::OnUpdate(double deltaTime) -> void {
  GameManager::Update(deltaTime);

  if (Input::IsKeyPressed(Key::D1)) {
    GameManager::SwitchScene(SceneName::Cutscene);
  } else if (Input::IsKeyPressed(Key::D2)) {
    GameManager::SwitchScene(SceneName::LVL_1);
  } else if (Input::IsKeyPressed(Key::D3)) {
    GameManager::SwitchScene(SceneName::MainMenu);
  }
  SceneManager::GetCurrentScene()->Update(deltaTime);
  SceneManager::GetCurrentScene()->Draw();
}

auto GameLayer::OnEvent(Engine::Event& event) -> void {
  EventDispatcher dispatcher(event);
  dispatcher.Dispatch< MouseButtonPressedEvent >(BIND_EVENT_FN(GameLayer::OnMouseButtonPress));
  dispatcher.Dispatch< MouseButtonReleasedEvent >(BIND_EVENT_FN(GameLayer::OnMouseButtonRelease));
  dispatcher.Dispatch< KeyPressedEvent >(BIND_EVENT_FN(GameLayer::OnKeyPress));
}

bool GameLayer::OnMouseButtonPress(MouseButtonPressedEvent& e) {
  if (e.GetMouseButton() == MouseCode::ButtonLeft) {
    auto mousePos = Input::GetMousePosition();
    mousePos.y    = Window::Get().GetHeight() - mousePos.y;
    SceneManager::GetCurrentScene()->OnMousePressed(mousePos);
  }

  return true;
}

bool GameLayer::OnMouseButtonRelease(MouseButtonReleasedEvent& e) {
  if (e.GetMouseButton() == MouseCode::ButtonLeft) {
    auto mousePos = Input::GetMousePosition();
    mousePos.y    = Window::Get().GetHeight() - mousePos.y;
    SceneManager::GetCurrentScene()->OnMouseReleased(mousePos);
  }
  return false;
}

bool GameLayer::OnKeyPress(Engine::KeyPressedEvent& e)
{
  SceneManager::GetCurrentScene()->OnKeyPressed((Key)e.GetKeyCode());
  return true;
}
