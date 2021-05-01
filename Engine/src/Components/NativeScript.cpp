#include "pch.h"
#include "NativeScript.h"
#include <Engine/IScript.h>
#include <ECS/EntityManager.h>

auto Engine::ScriptOrderOperator::operator()(const std::shared_ptr< IScript >& lhs,
                                             const std::shared_ptr< IScript >& rhs) const -> bool {
  return lhs->Priority() < rhs->Priority();
}

auto Engine::NativeScript::Attach(const std::shared_ptr< IScript >& script) -> void {
  if (script == nullptr) {
    return;
  }
  script->Entity(Entity());
  _scripts.insert(script);
  script->OnCreate();
}

auto Engine::NativeScript::Detach(const std::shared_ptr< IScript >& script) -> void {
  if (script == nullptr) {
    return;
  }
  if (_scripts.count(script) == 0) {
    return;
  }
  script->OnDestroy();
  _scripts.erase(script);
}

auto Engine::NativeScript::Update(float deltaTime) -> void {
  for (auto& script : _scripts) {
    script->Update(deltaTime);
  }
}

auto Engine::NativeScript::Entity() -> std::shared_ptr< ECS::Entity > {
  if (_entity == nullptr) {
    _entity = ECS::EntityManager::GetInstance().GetEntity(GetEntityID());
  }
  return _entity;
}
