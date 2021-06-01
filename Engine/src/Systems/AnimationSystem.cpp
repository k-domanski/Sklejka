#include "pch.h"
#include "AnimationSystem.h"
#include <ECS/EntityManager.h>

namespace Engine::Systems {
  using ECS::EntityManager;
  AnimationSystem::AnimationSystem(): m_MeshRenderer(nullptr) {
    AddSignature< Animator >();

    m_JointBuffer.BindToSlot(GL::UniformBlock::JointData);
  }
  auto AnimationSystem::Update(float deltaTime) -> void {
    for (auto& entity : _entities) {
      auto& animator = entity->GetComponent< Animator >();
      animator->UpdateAnimation(deltaTime * 5.0f);  // <- Why is this scaled by 5.0f?
      m_JointBuffer.SetData(animator->GetJointData());
    }
    /* WTF, I think it's not necessary */
    /*
    if (m_MeshRenderer == nullptr) {
      for (auto entityID : _entities) {
        auto mesh      = EntityManager::GetComponent< MeshRenderer >(entityID);
        m_MeshRenderer = mesh;
      }

    } else if (m_currentAnimator == nullptr) {
      for (auto entityID : _entities)
        m_currentAnimator = EntityManager::GetComponent< Animator >(entityID);
    }

    if (m_currentAnimator != nullptr) {
      m_currentAnimator->UpdateAnimation(deltaTime * 5.0f);
    }
    */
  }
}  // namespace Engine::Systems