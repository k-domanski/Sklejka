#include "pch.h"
#include "Animator.h"

namespace Engine {
	Animator::Animator(std::shared_ptr<Animation> animation)
		:m_CurrentAnimation(animation), m_CurrentTime(0.0f), m_DeltaTime(0.0f)
	{
		m_FinalBoneMatrices.reserve(100);
		for (int i = 0; i < 100; i++)
			m_FinalBoneMatrices.push_back(glm::mat4(1.0f));
	}
	void Animator::UpdateAnimation(float deltaTime)
	{
		m_DeltaTime = deltaTime;
		if (m_CurrentAnimation != nullptr)
		{
			m_CurrentTime += m_CurrentAnimation->GetTicksPerSecond() * deltaTime;
			m_CurrentTime = fmod(m_CurrentTime, m_CurrentAnimation->GetDuration());
			CalculateBoneTransform(m_CurrentAnimation->GetRootNode(), glm::mat4(1.0f));
		}
	}
	void Animator::PlayAnimation(std::shared_ptr<Animation> animation)
	{
		m_CurrentAnimation = animation;
		m_CurrentTime = 0.0f;
	}
	void Animator::CalculateBoneTransform(const AssimpNodeData& node, glm::mat4 parentTransform)
	{
		std::string nodeName = node.name;
		glm::mat4 nodeTransform = node.transformation;

		Bone* bone = m_CurrentAnimation->FindBone(nodeName);

		if (bone)
		{
			bone->Update(m_CurrentTime);
			nodeTransform = bone->GetLocalTransform();
		}

		glm::mat4 globalTransformation = parentTransform * nodeTransform;

		auto boneInfoMap = m_CurrentAnimation->GetBoneIDMap();
		if (boneInfoMap.find(nodeName) != boneInfoMap.end())
		{
			int index = boneInfoMap[nodeName].boneId;
			glm::mat4 offset = boneInfoMap[nodeName].inverseBindPose;
			m_FinalBoneMatrices[index] = globalTransformation * offset;
		}

		for (int i = 0; i < node.childrenCount; i++)
		{
			CalculateBoneTransform(node.children[i], globalTransformation);
		}
	}
}