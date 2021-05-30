#include "pch.h"
#include "ScriptSystem.h"
#include <Components/NativeScript.h>
#include <ECS/EntityManager.h>

namespace Engine::Systems {
  ScriptSystem::ScriptSystem() {
    AddSignature< NativeScript >();
  }

  auto ScriptSystem::Update(float deltaTime) -> void {
    for (auto id : _entities) {
      auto native_script = ECS::EntityManager::GetComponent< NativeScript >(id);
      native_script->Update(deltaTime);
    }
  }

  auto ScriptSystem::OnKeyPressed(Key key) -> void
  {
    for (auto id : _entities)
    {
      auto native_script = ECS::EntityManager::GetComponent< NativeScript >(id);
      native_script->OnKeyPressed(key);
    }
  }
}  // namespace Engine::Systems
