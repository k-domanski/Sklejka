#include "pch.h"
#include "AnimationSystem.h"
#include <ECS/EntityManager.h>

namespace Engine::Systems {
	using ECS::EntityManager;
	AnimationSystem::AnimationSystem() : m_JointsUniformSlot(4u), m_MeshRenderer(nullptr)
	{
		AddSignature<MeshRenderer>();
		for (int i = 0; i < 100; i++)
			m_JointTransforms.joints[i] = glm::mat4(1.0f);
	}
	auto AnimationSystem::Update(float deltaTime) -> void
	{
		if (m_MeshRenderer == nullptr)
		{
			for (auto entityID : _entities)
			{
				//m_currentAnimator = EntityManager::GetComponent<Animator>(entityID);
				auto mesh = EntityManager::GetComponent<MeshRenderer>(entityID);
				m_MeshRenderer = mesh;
			}
			
		}
		else if(m_Animation == nullptr)
		{
			m_Animation = std::make_shared<Animation>(m_MeshRenderer->GetModel()->GetFilepath(), m_MeshRenderer->GetModel());
			m_currentAnimator = std::make_shared<Animator>(m_Animation);
		}
		
		if (m_currentAnimator != nullptr)
		{
			m_currentAnimator->UpdateAnimation(deltaTime);
			auto finalMatrices = m_currentAnimator->GetFinalBoneMatrices();
			for (int i = 0; i < finalMatrices.size(); i++)
			{
				m_MeshRenderer->GetMaterial()->GetShader()->SetMatrix("u_Transforms[" + std::to_string(i) + "]", finalMatrices[i]);
				//m_JointTransforms.joints[i] = finalMatrices[i];
			}
				//m_JointsUniformBuffer.SetData(m_JointTransforms);
				//m_JointsUniformBuffer.BindToSlot(m_JointsUniformSlot);
		}
		int tets = 0;
	}
}