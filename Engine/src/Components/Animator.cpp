#include "pch.h"
#include "Animator.h"
#include <nlohmann/json.hpp>
#include <Utility/Utility.h>
#include <App/AssetManager.h>

namespace Engine {
	Animator::Animator()
		:Component("Animator")
	{
	}
	Animator::Animator(std::shared_ptr<Animation> animation)
		:Component("Animator"),m_CurrentAnimation(animation), m_CurrentTime(0.0f), m_DeltaTime(0.0f)
	{
		m_BoneInfoMap = m_CurrentAnimation->GetBoneIDMap();
		m_FinalBoneMatrices.reserve(100);
		for (int i = 0; i < 100; i++)
			m_FinalBoneMatrices.push_back(glm::mat4(1.0f));
	}
	Animator::Animator(std::shared_ptr<Renderer::Model> model)
		:Component("Animator"), m_CurrentTime(0.0f), m_DeltaTime(0.0f)
	{
		m_CurrentAnimation = std::make_shared<Animation>(model);
		m_BoneInfoMap = m_CurrentAnimation->GetBoneIDMap();
		m_FinalBoneMatrices.reserve(100);
		for (int i = 0; i < 100; i++)
			m_FinalBoneMatrices.push_back(glm::mat4(1.0f));
	}
	void Animator::UpdateAnimation(float deltaTime)
	{
		m_DeltaTime = deltaTime;
		if (m_CurrentAnimation != nullptr)
		{
			m_CurrentTime += m_CurrentAnimation->GetTicksPerSecond() * deltaTime * m_AnimationSpeed;
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

		if (m_BoneInfoMap.find(nodeName) != m_BoneInfoMap.end())
		{
			int index = m_BoneInfoMap[nodeName].boneId;
			glm::mat4 offset = m_BoneInfoMap[nodeName].inverseBindPose;
			m_FinalBoneMatrices[index] = globalTransformation * offset;
		}

		for (int i = 0; i < node.childrenCount; i++)
		{
			CalculateBoneTransform(node.children[i], globalTransformation);
		}
	}
	std::string Animator::SaveToJson()
	{
		using namespace nlohmann;
		json json = nlohmann::json{
			{"componentType", "animator"},
			{"model", Utility::StripToRelativePath(m_CurrentAnimation->GetFilePath())},
		};
		return json.dump(2);
	}
	void Animator::LoadFromJson(std::string filePath)
	{
		nlohmann::json json;
		if (filePath[0] == '{' || filePath[0] == '\n')  // HACK: Check if string is json
			json = nlohmann::json::parse(filePath.begin(), filePath.end());
		else {
			auto content = Utility::ReadTextFile(filePath);
			json = nlohmann::json::parse(content.begin(), content.end());
		}

		auto model = AssetManager::GetModel(json["model"]);
		m_CurrentAnimation = std::make_shared<Animation>(model);
		m_BoneInfoMap = m_CurrentAnimation->GetBoneIDMap();
		m_FinalBoneMatrices.reserve(100);
		for (int i = 0; i < 100; i++)
			m_FinalBoneMatrices.push_back(glm::mat4(1.0f));
	}
	void Animator::SetAnimation(std::shared_ptr<Renderer::Model> model)
	{
		if (model == nullptr)
		{
			m_CurrentAnimation = nullptr;
			return;
		}
		m_CurrentTime = 0.0f;
		m_DeltaTime = 0.0f;
		m_CurrentAnimation = std::make_shared<Animation>(model);
		m_BoneInfoMap = m_CurrentAnimation->GetBoneIDMap();
		m_FinalBoneMatrices.reserve(100);
		m_FinalBoneMatrices.clear();
		for (int i = 0; i < 100; i++)
			m_FinalBoneMatrices.push_back(glm::mat4(1.0f));
	}
}