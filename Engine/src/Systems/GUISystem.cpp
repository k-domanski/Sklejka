#include "pch.h"
#include "GUISystem.h"

#include "App/Window.h"
#include "ECS/EntityManager.h"
#include "Components/Transform.h"
#include "Components/UIRenderer.h"

Engine::Systems::GUISystem::GUISystem(): System() {
  AddSignature< Engine::Components::UIRenderer >();
  AddSignature< Engine::Transform >();

  auto size      = Engine::Window::Get().GetScreenSize();
  _camProj       = glm::ortho(0.0f, size.x, 0.0f, size.y, -1.0f, 0.0f);
  _breakChecking = false;
}

void Engine::Systems::GUISystem::Update(float deltaTime) {
  for (auto entityID : _entities) {
    auto transform = Engine::ECS::EntityManager::GetComponent< Engine::Transform >(entityID);
    auto renderer =
        Engine::ECS::EntityManager::GetComponent< Engine::Components::UIRenderer >(entityID);

    auto& elements = renderer->GetElements();

    for (auto ui_element : elements) {
      if (ui_element->IsActive())
        ui_element->Draw(transform->GetWorldMatrix(), _camProj);
    }
  }
}

auto Engine::Systems::GUISystem::OnWindowResize(glm::vec2 windowSize) -> void {
  _camProj = glm::ortho(0.0f, windowSize.x, 0.0f, windowSize.y, -1.0f, 0.0f);
}

auto Engine::Systems::GUISystem::HandleMouseRelease(glm::vec2 mousePosition) -> void {
  for (auto entityID : _entities) {
    auto transform = Engine::ECS::EntityManager::GetComponent< Engine::Transform >(entityID);
    auto renderer =
        Engine::ECS::EntityManager::GetComponent< Engine::Components::UIRenderer >(entityID);
    auto& buttons = renderer->GetButtons();

    for (auto button : buttons) {
      // if (button->IsActive())
      button->OnReleaseHandle(transform->WorldPosition() - glm::vec3(mousePosition, 0.0f));
    }
  }
}

auto Engine::Systems::GUISystem::BreakChecking(bool breakChecking) -> void {
  _breakChecking = breakChecking;
}

auto Engine::Systems::GUISystem::HandleMousePressed(glm::vec2 mousePosition) -> void {
  for (auto entityID : _entities) {
    auto transform = Engine::ECS::EntityManager::GetComponent< Engine::Transform >(entityID);
    auto renderer =
        Engine::ECS::EntityManager::GetComponent< Engine::Components::UIRenderer >(entityID);
    if (renderer != nullptr) {
      auto& buttons = renderer->GetButtons();

      for (auto button : buttons) {
        if (button->IsActive())
          button->OnPressHandle(transform->WorldPosition() - glm::vec3(mousePosition, 0.0f));
        if (_breakChecking) {
          LOG_DEBUG("Checking was broken");
          break;
        }
      }
    }
    if (_breakChecking) {
      LOG_DEBUG("Checking was broken");
      break;
    }
  }
  _breakChecking = false;
}
