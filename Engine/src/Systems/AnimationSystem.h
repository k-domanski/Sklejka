#pragma once
#include "ECS/ECS.h"
#include "Components/Animator.h"
#include "Components/MeshRenderer.h"
#include <GL/Buffer.h>

namespace Engine::Systems {
	using namespace Components;
	class AnimationSystem : public ECS::System {
	public:
		AnimationSystem();
		auto Update(float deltaTime) -> void override;
	private:
		uint32_t m_JointsUniformSlot{ 4u };
		std::shared_ptr<Animator> m_currentAnimator;
		std::shared_ptr<Animation> m_Animation;
		std::shared_ptr<MeshRenderer> m_MeshRenderer;
		GL::JointsUniformBuffer m_JointsUniformBuffer;
		GL::JointMatrix m_JointTransforms;
	};
}