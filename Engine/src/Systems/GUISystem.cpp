#include "pch.h"
#include "GUISystem.h"

#include "App/AssetManager.h"
#include "App/Input.h"
#include "App/Window.h"
#include "App/KeyCodes/GamepadCode.h"
#include "ECS/EntityManager.h"
#include "Components/Transform.h"
#include "Components/UIRenderer.h"
#include "GL/Context.h"

Engine::Systems::GUISystem::GUISystem(): System() {
  AddSignature< Engine::Components::UIRenderer >();
  AddSignature< Engine::Transform >();

  auto size          = Engine::Window::Get().GetScreenSize();
  _oldWindowSize     = glm::vec2(1600.0f, 900.0f);//refered size   // size;
  _camProj           = glm::ortho(0.0f, size.x, 0.0f, size.y, -1.0f, 0.0f);
  _breakChecking     = false;
  _useMouse          = true;
  _DPADUpLastFrame   = false;
  _DPADDownLastFrame = false;
  _gamepadALastFrame = false;
}

void Engine::Systems::GUISystem::Update(float deltaTime) {
  auto DPADUp   = Input::IsGamepadButtonPressed(GamepadCode::BUTTON_DPAD_UP);
  auto DPADDown = Input::IsGamepadButtonPressed(GamepadCode::BUTTON_DPAD_DOWN);
  auto A        = Input::IsGamepadButtonPressed(GamepadCode::BUTTON_A);

  auto mousePos = Input::GetMousePosition();
  mousePos.y    = Window::Get().GetHeight() - mousePos.y;

  GL::Context::DepthTest(false);
  GL::Context::EnableBlending(true);
  GL::Context::SetBlendFunction(GL::BlendFunc::SrcAlpha, GL::BlendFunc::OneMinusSrcAlpha);
  GL::Context::BindFramebuffer(GL::FramebufferTarget::ReadWrite, 0);
  // GL::Context::ClearBuffers(GL::BufferBit::Color);

  for (auto entityID : _entities) {
    auto transform = Engine::ECS::EntityManager::GetComponent< Engine::Transform >(entityID);
    auto renderer =
        Engine::ECS::EntityManager::GetComponent< Engine::Components::UIRenderer >(entityID);
    if (renderer == nullptr)
      continue;
    if (_useMouse) {
      auto& buttons = renderer->GetButtons();
      for (auto button : buttons) {
        button->IsSelected(button->IsMouseOverButton(
            glm::vec3(mousePos, 0.0f)
            - (transform->WorldPosition() + glm::vec3(button->Offset(), 0.0f))));
      }
    } else {
      if (DPADUp && !_DPADUpLastFrame) {
        renderer->SelectPrevious();
      }
      if (DPADDown && !_DPADDownLastFrame) {
        renderer->SelectNext();
      }
      if (A && !_gamepadALastFrame) {
        renderer->TriggerOnPressed();
      }
    }

    auto& elements = renderer->GetElements();

    for (auto ui_element : elements) {
      if (ui_element->IsActive())
        ui_element->Draw(transform->GetWorldMatrix(), _camProj);
    }
  }

  if (A && !_gamepadALastFrame || DPADUp && !_DPADUpLastFrame || DPADDown && !_DPADDownLastFrame) {
    for (auto entityID : _entities) {
      auto transform = Engine::ECS::EntityManager::GetComponent< Engine::Transform >(entityID);
      auto renderer =
          Engine::ECS::EntityManager::GetComponent< Engine::Components::UIRenderer >(entityID);
      if (renderer == nullptr)
        continue;
      if (_useMouse)
        renderer->SelectButton(0);
    }
    _useMouse = false;
  }

  _DPADUpLastFrame   = DPADUp;
  _DPADDownLastFrame = DPADDown;
  _gamepadALastFrame = A;
  // to not trigger actions same frame
}

auto Engine::Systems::GUISystem::OnWindowResize(glm::vec2 windowSize) -> void {
  _camProj   = glm::ortho(0.0f, windowSize.x, 0.0f, windowSize.y, -1.0f, 0.0f);
  auto ratio = windowSize / _oldWindowSize;
  for (auto entity : _entities) {
    auto renderer  = entity->GetComponent< Components::UIRenderer >();
    auto transform = entity->GetComponent< Transform >();
    renderer->OnWindowResize(ratio);
    transform->Position(transform->Position() * glm::vec3(ratio, 0.0f));
  }
  _oldWindowSize = windowSize;
}

auto Engine::Systems::GUISystem::HandleMouseRelease(glm::vec2 mousePosition) -> void {
  _useMouse = true;
  for (auto entityID : _entities) {
    auto transform = Engine::ECS::EntityManager::GetComponent< Engine::Transform >(entityID);
    auto renderer =
        Engine::ECS::EntityManager::GetComponent< Engine::Components::UIRenderer >(entityID);
    if (renderer != nullptr) {
      auto& buttons = renderer->GetButtons();

      for (auto button : buttons) {
        if (button->IsActive())
          button->OnReleaseHandle((transform->WorldPosition() + glm::vec3(button->Offset(), 0.0f))
                                  - glm::vec3(mousePosition, 0.0f));
        if (_breakChecking) {
          break;
        }
      }
    }
    if (_breakChecking) {
      break;
    }
  }
  _breakChecking = false;
}

auto Engine::Systems::GUISystem::BreakChecking(bool breakChecking) -> void {
  _breakChecking = breakChecking;
}

auto Engine::Systems::GUISystem::HandleMousePressed(glm::vec2 mousePosition) -> void {
  _useMouse = true;
  for (auto entityID : _entities) {
    auto transform = Engine::ECS::EntityManager::GetComponent< Engine::Transform >(entityID);
    auto renderer =
        Engine::ECS::EntityManager::GetComponent< Engine::Components::UIRenderer >(entityID);
    if (renderer != nullptr) {
      auto& buttons = renderer->GetButtons();

      for (auto button : buttons) {
        if (button->IsActive())
          button->OnPressHandle((transform->WorldPosition() + glm::vec3(button->Offset(), 0.0f))
                                - glm::vec3(mousePosition, 0.0f));
        if (_breakChecking) {
          break;
        }
      }
    }
    if (_breakChecking) {
      break;
    }
  }
  _breakChecking = false;
}