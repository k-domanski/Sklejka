#include "pch.h"
#include "Animation.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"

namespace Engine {
	Animation::Animation(const std::string& animationPath, std::shared_ptr<Renderer::Model> model)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(animationPath, aiProcess_Triangulate);
		auto animation = scene->mAnimations[0];
		m_Duration = animation->mDuration;
		m_TicksPerSecond = animation->mTicksPerSecond;
		ReadHierarchyData(m_RootNode, scene->mRootNode);
		ReadMissingBones(animation, model);
	}
	Animation::~Animation()
	{
	}
	Bone* Animation::FindBone(const std::string& name)
	{
		auto iter = std::find_if(m_Bones.begin(), m_Bones.end(), [&](const Bone& bone) {return bone.GetBoneName() == name; });
		if (iter == m_Bones.end())
			return nullptr;
		else
			return &(*iter);
	}
	void Animation::ReadMissingBones(const aiAnimation* animation, std::shared_ptr<Renderer::Model> model)
	{
		int size = animation->mNumChannels;
		auto& boneInfoMap = model->GetBoneInfoMap();
		unsigned int boneCount = model->GetBoneCount();

		for (int i = 0; i < size; i++)
		{
			auto channel = animation->mChannels[i];
			std::string boneName = channel->mNodeName.data;
			if (boneInfoMap.find(boneName) == boneInfoMap.end())
			{
				boneInfoMap[boneName].boneId = boneCount;
				boneCount++;
			}
			m_Bones.push_back(Bone(channel->mNodeName.data, boneInfoMap[channel->mNodeName.data].boneId, channel));
		}
		m_BoneMapInfo = boneInfoMap;
	}
	void Animation::ReadHierarchyData(AssimpNodeData& destination, const aiNode* source)
	{
		destination.name = source->mName.data;
		destination.transformation = Renderer::Model::aiMat4ToGlmMat4(source->mTransformation);
		destination.childrenCount = source->mNumChildren;

		for (int i = 0; i < source->mNumChildren; i++)
		{
			AssimpNodeData newData;
			ReadHierarchyData(newData, source->mChildren[i]);
			destination.children.push_back(newData);
		}
	}
}