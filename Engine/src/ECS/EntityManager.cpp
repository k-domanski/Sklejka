#include "pch.h"
#include "EntityManager.h"
#include <random>

namespace Engine::ECS {
  auto EntityManager::UpdateSystem(SystemTypeID systemID) -> void {
    for (const auto entity : _entities) {
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
      auto& s = _registeredSystems[systemID]->_signatures;
      if (_registeredSystems[systemID]->SignatureMatch(*entitySignatures)) {
        AddToSystem(systemID, entity->GetID());
      }
    }
  }

  auto EntityManager::UpdateEntity(std::shared_ptr< Entity > entity) -> void {
    for (auto [systemID, system] : _registeredSystems) {
      int correctSignatures        = 0;
      const auto& entitySignatures = entity->GetSignature();
      for (auto signature : *entitySignatures) {
        if (system->ContainsSignature(signature))
          correctSignatures++;
      }
      if (correctSignatures == system->_signatures.size())
        AddToSystem(systemID, entity->GetID());
    }
  }

  auto EntityManager::BelongsToSystem(SystemTypeID systemID, EntityID entityID) -> bool {
    return _registeredSystems[systemID]->_entities.find(entityID)
           != _registeredSystems[systemID]->_entities.end();
  }

  auto EntityManager::AddToSystem(SystemTypeID systemID, EntityID entityID) -> void {
    // if (BelongsToSystem(systemID, entityID))
    // return;
    //_registeredSystems[systemID]->_entities.insert(entityID);
    _registeredSystems[systemID]->AddEntity(entityID);
  }

  auto EntityManager::CreateEntity() -> std::shared_ptr< Entity > {
    std::random_device dv;
    std::mt19937_64 mt(dv());
    std::shared_ptr< Entity > entity = std::make_shared< Entity >();
    entity->_entityID                = static_cast< EntityID >(mt());
    _entities.push_back(entity);
    return entity;
    // return entity;
  }
  auto EntityManager::CreateEntity(EntityID id) -> std::shared_ptr< Entity > {
    std::shared_ptr< Entity > entity = std::make_shared< Entity >();
    entity->_entityID                = id;
    _entities.push_back(entity);
    return entity;
    // return entity;
  }
  auto EntityManager::GetEntity(EntityID id) -> std::shared_ptr< Entity > {
    const auto it = std::find_if(_entities.begin(), _entities.end(),
                                 [id](auto ent) { return ent->_entityID == id; });
    assert(it != _entities.end());

    return *it;
  }

  auto EntityManager::Update() -> void {
    for (auto [id, system] : _registeredSystems) {
      system->Update();
    }
  }

  auto EntityManager::Clear() -> void {
    _entities.clear();
    _componentLists.clear();
    _registeredSystems.clear();
  }

  /* auto EntityManager::Draw() -> void {
    for (auto [id, system] : _registeredSystems) {
      system->Draw();
    }
  }*/
}  // namespace Engine::ECS
