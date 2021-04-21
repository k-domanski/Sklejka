#include "pch.h"
#include "EntityManager.h"
#include <random>

namespace Engine::ECS {
  auto EntityManager::UpdateSystem(SystemTypeID systemID) -> void {
    for (const auto entity : SceneManager::GetCurrentScene()->_entities) {
      if (BelongsToSystem(systemID, entity->GetID()))
        continue;
      int signatureCorrect  = 0;
      auto entitySignatures = entity->GetSignature();
      /*for (auto& entitySignature : *entitySignatures) {
        auto rs = _registeredSystems[systemID];
        if (rs->ContainsSignature(entitySignature)) {
          signatureCorrect++;
        }
      }
      if (signatureCorrect == _registeredSystems[systemID]->_signatures.size()) {
        AddToSystem(systemID, entity->GetID());
      }*/
      auto& s = SceneManager::GetCurrentScene()->_registeredSystems[systemID]->_signatures;
      if (SceneManager::GetCurrentScene()->_registeredSystems[systemID]->SignatureMatch(
              *entitySignatures)) {
        AddToSystem(systemID, entity->GetID());
      }
    }
  }

  auto EntityManager::UpdateEntity(std::shared_ptr< Entity > entity) -> void {
    for (auto [systemID, system] : SceneManager::GetCurrentScene()->_registeredSystems) {
      int correctSignatures        = 0;
      const auto& entitySignatures = entity->GetSignature();
      for (auto signature : *entitySignatures) {
        if (system->ContainsSignature(signature))
          correctSignatures++;
      }
      if (correctSignatures == system->_signatures.size())
        AddToSystem(systemID, entity->GetID());
      else
        RemoveFromSystem(systemID, entity->GetID());
    }
  }

  auto EntityManager::BelongsToSystem(SystemTypeID systemID, EntityID entityID) -> bool {
    return SceneManager::GetCurrentScene()->_registeredSystems[systemID]->_entities.find(entityID)
           != SceneManager::GetCurrentScene()->_registeredSystems[systemID]->_entities.end();
  }

  auto EntityManager::AddToSystem(SystemTypeID systemID, EntityID entityID) -> void {
    // if (BelongsToSystem(systemID, entityID))
    // return;
    //_registeredSystems[systemID]->_entities.insert(entityID);
    SceneManager::GetCurrentScene()->_registeredSystems[systemID]->AddEntity(entityID);
  }

  auto EntityManager::RemoveFromSystem(SystemTypeID systemID, EntityID entityID) -> void {
    SceneManager::GetCurrentScene()->_registeredSystems[systemID]->RemoveEntity(entityID);
  }

  auto EntityManager::CreateEntity() -> std::shared_ptr< Entity > {
    std::random_device dv;
    std::mt19937_64 mt(dv());
    std::shared_ptr< Entity > entity = std::make_shared< Entity >();
    entity->_entityID                = static_cast< EntityID >(mt());
    SceneManager::GetCurrentScene()->_entities.push_back(entity);
    return entity;
    // return entity;
  }
  auto EntityManager::CreateEntity(EntityID id) -> std::shared_ptr< Entity > {
    std::shared_ptr< Entity > entity = std::make_shared< Entity >();
    entity->_entityID                = id;
    SceneManager::GetCurrentScene()->_entities.push_back(entity);
    return entity;
    // return entity;
  }
  auto EntityManager::GetEntity(EntityID id) -> std::shared_ptr< Entity > {
    const auto it = std::find_if(SceneManager::GetCurrentScene()->_entities.begin(),
                                 SceneManager::GetCurrentScene()->_entities.end(),
                                 [id](auto ent) { return ent->_entityID == id; });
    assert(it != SceneManager::GetCurrentScene()->_entities.end());

    return *it;
  }

  auto EntityManager::RemoveEntity(EntityID id) -> void {
    for (auto& system : SceneManager::GetCurrentScene()->_registeredSystems) {
      RemoveFromSystem(system.first, id);
    }
    auto entity = GetEntity(id);
    for (auto signature : *entity->_signature) {
      SceneManager::GetCurrentScene()->_componentLists[signature]->Remove(id);
    }
    entity->_signature->clear();

    auto it = std::find(SceneManager::GetCurrentScene()->_entities.begin(),
                        SceneManager::GetCurrentScene()->_entities.end(), entity);
    if (it != SceneManager::GetCurrentScene()->_entities.end())
      SceneManager::GetCurrentScene()->_entities.erase(it);
  }

  auto EntityManager::Update(float deltaTime) -> void {
    for (auto [id, system] : SceneManager::GetCurrentScene()->_registeredSystems) {
      system->Update(deltaTime);
    }
  }

  auto EntityManager::Clear() -> void {
    SceneManager::GetCurrentScene()->_entities.clear();
    SceneManager::GetCurrentScene()->_componentLists.clear();
    SceneManager::GetCurrentScene()->_registeredSystems.clear();
  }

  /* auto EntityManager::Draw() -> void {
    for (auto [id, system] : _registeredSystems) {
      system->Draw();
    }
  }*/
}  // namespace Engine::ECS
