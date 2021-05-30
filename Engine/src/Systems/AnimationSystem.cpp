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
		}
	}
}