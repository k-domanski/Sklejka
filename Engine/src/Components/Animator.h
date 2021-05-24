#pragma once
#include "ECS/ECS.h"
#include "Renderer/Animation/Animation.h"

namespace Engine {
	//class Animation {
	//private:
	//
	//	struct KeyFrame {
	//		float timeStamp;
	//	};
	//
	//	std::vector<KeyFrame> keyFrames;
	//
	//};
	//class Animator : public ECS::Component {
	//private:
	//	Animation m_CurrentAnimation;
	//	float m_AnimationTime;
	//};
	class Animator {
	public:
		Animator(std::shared_ptr<Animation> animation);
		void UpdateAnimation(float deltaTime);
		void PlayAnimation(std::shared_ptr<Animation> animation);
		void CalculateBoneTransform(const AssimpNodeData& node, glm::mat4 parentTransform);
		std::vector<glm::mat4> GetFinalBoneMatrices() { return m_FinalBoneMatrices; }
	private:
		std::vector<glm::mat4> m_FinalBoneMatrices;
		std::shared_ptr<Animation> m_CurrentAnimation;
		float m_CurrentTime;
		float m_DeltaTime;

	};
}