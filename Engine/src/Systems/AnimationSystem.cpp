#include "pch.h"
#include "AnimationSystem.h"
#include <ECS/EntityManager.h>

namespace Engine::Systems {
	using ECS::EntityManager;
	AnimationSystem::AnimationSystem() : m_MeshRenderer(nullptr)
	{
		AddSignature<Animator>();
	}
	auto AnimationSystem::Update(float deltaTime) -> void
	{
		if (m_MeshRenderer == nullptr)
		{
			for (auto entityID : _entities)
			{
				auto mesh = EntityManager::GetComponent<MeshRenderer>(entityID);
				m_MeshRenderer = mesh;
			}
			
		}
		else if(m_currentAnimator == nullptr)
		{
			for (auto entityID : _entities)
				m_currentAnimator = EntityManager::GetComponent<Animator>(entityID);
		}
		
		if (m_currentAnimator != nullptr)
		{
			m_currentAnimator->UpdateAnimation(deltaTime * 5.0f);
			auto finalMatrices = m_currentAnimator->GetFinalBoneMatrices();
			for (int i = 0; i < finalMatrices.size(); i++)
			{
				m_MeshRenderer->GetMaterial()->GetShader()->SetMatrix("u_Transforms[" + std::to_string(i) + "]", finalMatrices[i]);
			}
		}
	}
}