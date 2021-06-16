#include "pch.h"
#include "NativeScript.h"
#include <Engine/IScript.h>
#include <ECS/EntityManager.h>

auto Engine::ScriptOrderOperator::operator()(const std::shared_ptr< IScript >& lhs,
                                             const std::shared_ptr< IScript >& rhs) const -> bool {
  return lhs->Priority() < rhs->Priority();
}

namespace Engine {
  NativeScript::NativeScript()
      : ECS::Component("Native Script", ECS::GetComponentTypeID< NativeScript >()) {
  }
  auto NativeScript::Attach(const std::shared_ptr< IScript >& script) -> void {
    if (script == nullptr) {
      return;
    }
    script->Entity(Entity());
    _scripts.insert(script);
    script->OnCreate();
  }

  auto NativeScript::Detach(const std::shared_ptr< IScript >& script) -> void {
    if (script == nullptr) {
      return;
    }
    if (_scripts.count(script) == 0) {
      return;
    }
    script->OnDestroy();
    _scripts.erase(script);
  }

  auto NativeScript::Update(float deltaTime) -> void {
    auto scripts_cpy = _scripts;
    for (auto& script : scripts_cpy) {
      script->Update(deltaTime);
    }
  }

  auto NativeScript::OnKeyPressed(Key key) -> void {
    for (auto& script : _scripts) {
      script->OnKeyPressed(key);
    }
  }

  auto NativeScript::OnCollisionEnter(const std::shared_ptr< Components::Collider >& collider)
      -> void {
    if (_scripts.empty()) {
      return;
    }
    auto scripts_cpy = _scripts;
    for (auto& script : scripts_cpy) {
      script->OnCollisionEnter(collider);
    }
  }

  auto NativeScript::OnDestroy() -> void {
    return;
    for (auto& script : _scripts) {
      script->OnDestroy();
    }
    _scripts.clear();
  }

  auto NativeScript::Entity() -> std::shared_ptr< ECS::Entity > {
    if (_entity == nullptr) {
      _entity = ECS::EntityManager::GetInstance().GetEntity(GetEntityID());
    }
    return _entity;
  }
}  // namespace Engine
