#include "pch.h"
#include "GUISystem.h"

#include "App/Window.h"
#include "ECS/EntityManager.h"
#include "Components/Transform.h"
#include "Components/UIRenderer.h"

Engine::Systems::GUISystem::GUISystem(): System() {
  AddSignature< Engine::Components::UIRenderer >();
  AddSignature< Engine::Transform >();

  auto size = Engine::Window::Get().GetScreenSize() * 0.5f;
  _camProj  = glm::ortho(-size.x, size.x, -size.y, size.y, 0.0f, 1.0f);
}

void Engine::Systems::GUISystem::Update(float deltaTime) {
  for (auto entityID : _entities) {
    auto transform = Engine::ECS::EntityManager::GetComponent< Engine::Transform >(entityID);
    auto renderer =
        Engine::ECS::EntityManager::GetComponent< Engine::Components::UIRenderer >(entityID);

    auto& elements = renderer->GetElements();

    for (auto ui_element : elements) {
      ui_element->Draw(transform->GetWorldMatrix(), _camProj);
    }
  }
}

auto Engine::Systems::GUISystem::OnWindowResize(glm::vec2 windowSize) -> void {
  _camProj = glm::ortho(0.0f, windowSize.x, 0.0f, windowSize.y, -1.0f, 0.0f);
}
