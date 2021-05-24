#pragma once
#include "assimp/scene.h"

namespace Engine
{
	struct KeyTransform {
		glm::vec3 position;
		glm::quat rotation;
		glm::vec3 scale;
		float timeStamp;
	};
	struct KeyPosition {
		glm::vec3 position;
		float timeStamp;
	};
	struct KeyRotation {
		glm::quat rotation;
		float timeStamp;
	};
	struct KeyScale {
		glm::vec3 scale;
		float timeStamp;
	};
	class Bone {
	public:
		Bone(const std::string& name, int ID, const aiNodeAnim* channel);
		void Update(float animationTime);

		glm::mat4 GetLocalTransform() { return m_LocalTransform; }
		std::string GetBoneName() const { return m_Name; }
		unsigned int GetBoneID() { return m_ID; }

		unsigned int GetPositionIndex(float animationTime);
		unsigned int GetRotationIndex(float animationTime);
		unsigned int GetScaleIndex(float animationTime);


	private:
		std::vector<KeyPosition> m_Positions;
		std::vector<KeyRotation> m_Rotations;
		std::vector<KeyScale> m_Scale;
		unsigned int m_NumPositions;
		unsigned int m_NumRotations;
		unsigned int m_NumScales;

		std::string m_Name;
		unsigned int m_ID;
		glm::mat4 m_LocalTransform;

		float GetScaleFactor(float lastTime, float nextTime, float animationTime);
		glm::mat4 InterpolatePosition(float animationTime);
		glm::mat4 InterpolateRotation(float animationTime);
		glm::mat4 InterpolateScale(float animationTime);
	public:
		static glm::vec3 GetGLMVec3(const aiVector3D& vector);
		static glm::quat GetGLMQuat(const aiQuaternion& quaternion);
	};
}