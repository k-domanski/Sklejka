#include "pch.h"
#include "EntityManager.h"
#include <random>
//#include "ECS.h"
//#include "Engine/SceneManager.h"

namespace Engine::ECS {
  auto EntityManager::UpdateSystem(SystemTypeID systemID) -> void {
    for (const auto entity : SceneManager::GetCurrentScene()->_entities) {
      if (BelongsToSystem(systemID, entity))
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
        AddToSystem(systemID, entity);
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
        AddToSystem(systemID, entity);
      else
        RemoveFromSystem(systemID, entity);
    }
  }

  auto EntityManager::BelongsToSystem(SystemTypeID systemID,
                                      const std::shared_ptr< Entity >& entity) -> bool {
    const auto& entities = SceneManager::GetCurrentScene()->_registeredSystems[systemID]->_entities;
    auto it              = std::find(entities.begin(), entities.end(), entity);
    return it != entities.end();
  }

  auto EntityManager::AddToSystem(SystemTypeID systemID, const std::shared_ptr< Entity >& entity)
      -> void {
    // if (BelongsToSystem(systemID, entityID))
    // return;
    //_registeredSystems[systemID]->_entities.insert(entityID);
    SceneManager::GetCurrentScene()->_registeredSystems[systemID]->AddEntity(entity);
  }

  auto EntityManager::RemoveFromSystem(SystemTypeID systemID,
                                       const std::shared_ptr< Entity >& entity) -> void {
    SceneManager::GetCurrentScene()->_registeredSystems[systemID]->RemoveEntity(entity);
  }

  auto EntityManager::CreateEntity() -> std::shared_ptr< Entity > {
    std::random_device dv;
    std::mt19937_64 mt(dv());
    std::shared_ptr< Entity > entity = std::make_shared< Entity >();
    entity->_entityID                = static_cast< EntityID >(mt());
    SceneManager::GetCurrentScene()->_entities.push_back(entity);
    entity->_entity = entity;
    return entity;
    // return entity;
  }
  auto EntityManager::CreateEntity(EntityID id) -> std::shared_ptr< Entity > {
    std::shared_ptr< Entity > entity = std::make_shared< Entity >();
    entity->_entityID                = id;
    SceneManager::GetCurrentScene()->_entities.push_back(entity);
    entity->_entity = entity;
    return entity;
    // return entity;
  }
  auto EntityManager::GetEntity(EntityID id) -> std::shared_ptr< Entity > {
    const auto it = std::find_if(SceneManager::GetCurrentScene()->_entities.begin(),
                                 SceneManager::GetCurrentScene()->_entities.end(),
                                 [id](auto ent) { return ent->_entityID == id; });

    if (it == SceneManager::GetCurrentScene()->_entities.end()) {
      return nullptr;
    }
    return *it;
  }

  auto EntityManager::RemoveEntity(const std::shared_ptr< Entity >& entity) -> void {
    for (auto& system : SceneManager::GetCurrentScene()->_registeredSystems) {
      RemoveFromSystem(system.first, entity);
    }
    for (auto signature : *(entity->_signature)) {
      SceneManager::GetCurrentScene()->_componentLists[signature]->Remove(entity);
    }
    // entity->_signature->clear();

    auto it = std::find(SceneManager::GetCurrentScene()->_entities.begin(),
                        SceneManager::GetCurrentScene()->_entities.end(), entity);
    if (it != SceneManager::GetCurrentScene()->_entities.end())
      SceneManager::GetCurrentScene()->_entities.erase(it);
  }

  auto EntityManager::GetAllComponents(EntityID id) -> std::vector< std::shared_ptr< Component > > {
    std::vector< std::shared_ptr< Component > > res;
    auto entity = GetEntity(id);
    for (auto signature : *entity->_signature) {
      auto list = SceneManager::GetCurrentScene()->_componentLists[signature];
      // Hack: need to get every component somehow for serialization
      auto riskyList = std::static_pointer_cast< ComponentList< Component > >(list);
      res.push_back(riskyList->GetComponent(id));
    }

    return res;
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

  auto EntityManager::InjectEntity(const std::shared_ptr< Entity >& entity) -> void {
    auto& entities = SceneManager::GetCurrentScene()->_entities;
    auto it        = std::find(entities.begin(), entities.end(), entity);
    if (it != entities.end()) {
      LOG_WARN("Entity already in the scene: {}: {}", entity->Name(), entity->GetID());
      return;
    }
    entities.push_back(entity);
  }

  /* auto EntityManager::Draw() -> void {
    for (auto [id, system] : _registeredSystems) {
      system->Draw();
    }
  }*/
}  // namespace Engine::ECS
