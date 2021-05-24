#pragma once
#include "Bone.h"
#include"Renderer/Model.h"

namespace Engine {
	struct AssimpNodeData {
		glm::mat4 transformation;
		std::string name;
		int childrenCount;
		std::vector<AssimpNodeData> children;
	};

	class Animation {
	public:
		Animation() = default;
		Animation(const std::string& animationPath, std::shared_ptr<Renderer::Model> model);
		~Animation();
		Bone* FindBone(const std::string& name);
		float GetTicksPerSecond() { return m_TicksPerSecond; }
		float GetDuration() { return m_Duration; }
		const AssimpNodeData& GetRootNode() { return m_RootNode; }
		const std::map<std::string, Renderer::BoneInfo>& GetBoneIDMap() { return m_BoneMapInfo; }


	private:
		void ReadMissingBones(const aiAnimation* animation, std::shared_ptr<Renderer::Model> model);
		void ReadHierarchyData(AssimpNodeData& destination, const aiNode* source);

		float m_Duration;
		float m_TicksPerSecond;
		AssimpNodeData m_RootNode;
		std::vector<Bone> m_Bones;
		std::map<std::string, Renderer::BoneInfo> m_BoneMapInfo;
	};
}