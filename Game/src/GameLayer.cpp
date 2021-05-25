#include <GameLayer.h>
#include <filesystem>
#include <Components/NativeScript.h>
#include <Scripts/CameraController.h>
#include <Scripts/PlayerController.h>
#include <Scripts/PlayerRect.h>
#include <Systems/SceneGraph.h>

#include <Scripts/ShadowTarget.h>
#include <Scripts/FlightTimer.h>

#include <GameManager.h>
#include "Scripts/StartTimer.h"

using namespace Engine;
GameLayer::GameLayer(): Engine::Layer("Game") {
}

auto GameLayer::OnAttach() -> void {
  LOG_TRACE("Current dir: {}", std::filesystem::current_path().string());
  GameManager::SwitchScene(SceneName::LVL_1);

  auto scene = SceneManager::GetCurrentScene();
  //SetupPlayer(scene);

  // GameManager::ShowLoadingScreen();
}

auto GameLayer::OnDetach() -> void {
}

auto GameLayer::OnUpdate(double deltaTime) -> void {
  if (Input::IsKeyPressed(Key::D1)) {
    GameManager::SwitchScene(SceneName::Loading);
  } else if (Input::IsKeyPressed(Key::D2)) {
    GameManager::SwitchScene(SceneName::LVL_1);
  }

  SceneManager::GetCurrentScene()->Update(deltaTime);
  SceneManager::GetCurrentScene()->Draw();
}

auto GameLayer::OnEvent(Engine::Event& event) -> void {
  EventDispatcher dispatcher(event);
  dispatcher.Dispatch< MouseButtonPressedEvent >(BIND_EVENT_FN(GameLayer::OnMouseButtonPress));
  dispatcher.Dispatch< MouseButtonReleasedEvent >(BIND_EVENT_FN(GameLayer::OnMouseButtonRelease));
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
