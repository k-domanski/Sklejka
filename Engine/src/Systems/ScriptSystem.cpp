#include "pch.h"
#include "ScriptSystem.h"
#include <Components/NativeScript.h>
#include <ECS/EntityManager.h>

Engine::ScriptSystem::ScriptSystem() {
  AddSignature< NativeScript >();
}

auto Engine::ScriptSystem::Update(float deltaTime) -> void {
  for (auto id : _entities) {
    auto native_script = ECS::EntityManager::GetComponent< NativeScript >(id);
    native_script->Update(deltaTime);
  }
}
