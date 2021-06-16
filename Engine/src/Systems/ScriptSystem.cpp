#include "pch.h"
#include "ScriptSystem.h"
#include <Components/NativeScript.h>
#include <ECS/EntityManager.h>

namespace Engine::Systems {
  ScriptSystem::ScriptSystem() {
    AddSignature< NativeScript >();
  }

  auto ScriptSystem::Update(float deltaTime) -> void {
    if (_entities.empty())
      return;
    auto entities_cpy = _entities;
    for (auto id : entities_cpy) {
      auto native_script = ECS::EntityManager::GetComponent< NativeScript >(id);
      native_script->Update(deltaTime);
    }
  }

  auto ScriptSystem::OnKeyPressed(Key key) -> void {
    if (_entities.empty()) {
      return;
    }
    auto entities_cpy = _entities;
    for (auto& entity : entities_cpy) {
      if (entity != nullptr) {
        auto native_script = ECS::EntityManager::GetComponent< NativeScript >(entity);
        native_script->OnKeyPressed(key);
      }
    }
  }
}  // namespace Engine::Systems
