#include "pch.h"
#include "Bone.h"

namespace Engine {
	Bone::Bone(const std::string& name, int ID, const aiNodeAnim* channel)
		:m_Name(name), m_ID(ID)
	{
		m_NumPositions = channel->mNumPositionKeys;
		for (unsigned int positionIndex = 0; positionIndex < m_NumPositions; ++positionIndex)
		{
			aiVector3D aiPosition = channel->mPositionKeys[positionIndex].mValue;
			float timeStamp = channel->mPositionKeys[positionIndex].mTime;
			KeyPosition key;
			key.position = GetGLMVec3(aiPosition);
			key.timeStamp = timeStamp;
			m_Positions.push_back(key);
		}
		m_NumRotations = channel->mNumRotationKeys;
		for (unsigned int rotationIndex = 0; rotationIndex < m_NumRotations; ++rotationIndex)
		{
			aiQuaternion aiRotation = channel->mRotationKeys[rotationIndex].mValue;
			float timeStamp = channel->mRotationKeys[rotationIndex].mTime;
			KeyRotation key;
			key.rotation = GetGLMQuat(aiRotation);
			key.timeStamp = timeStamp;
			m_Rotations.push_back(key);
		}
		m_NumScales = channel->mNumScalingKeys;
		for (unsigned int scaleIndex = 0; scaleIndex < m_NumScales; ++scaleIndex)
		{
			aiVector3D aiScale = channel->mScalingKeys[scaleIndex].mValue;
			float timeStamp = channel->mScalingKeys[scaleIndex].mTime;
			KeyScale key;
			key.scale = GetGLMVec3(aiScale);
			key.timeStamp = timeStamp;
			m_Scale.push_back(key);
		}
	}
	void Bone::Update(float animationTime)
	{
		glm::mat4 translation = InterpolatePosition(animationTime);
		glm::mat4 rotation = InterpolateRotation(animationTime);
		glm::mat4 scale = InterpolateScale(animationTime);
		m_LocalTransform = translation * rotation * scale;
	}
	unsigned int Bone::GetPositionIndex(float animationTime)
	{
		for (unsigned int i = 0; i < m_NumPositions - 1; ++i)
		{
			if (animationTime < m_Positions[i + 1].timeStamp)
				return i;
		}
	}
	unsigned int Bone::GetRotationIndex(float animationTime)
	{
		for (unsigned int i = 0; i < m_NumRotations - 1; ++i)
		{
			if (animationTime < m_Rotations[i + 1].timeStamp)
				return i;
		}
	}
	unsigned int Bone::GetScaleIndex(float animationTime)
	{
		for (unsigned int i = 0; i < m_NumScales - 1; ++i)
		{
			if (animationTime < m_Scale[i + 1].timeStamp)
				return i;
		}
	}
	float Bone::GetScaleFactor(float lastTime, float nextTime, float animationTime)
	{
		float scaleFactor = 0.0f;
		float midwayLenght = animationTime - lastTime;
		float frameDiff = nextTime - lastTime;
		scaleFactor = midwayLenght / frameDiff;
		return scaleFactor;
	}
	glm::mat4 Bone::InterpolatePosition(float animationTime)
	{
		if (1 == m_NumPositions)
			return glm::translate(glm::mat4(1.0f), m_Positions[0].position);

		int p0Index = GetPositionIndex(animationTime);
		int p1Index = p0Index + 1;

		float scaleFactor = GetScaleFactor(m_Positions[p0Index].timeStamp, m_Positions[p1Index].timeStamp, animationTime);
		glm::vec3 finalPosition = glm::mix(m_Positions[p0Index].position, m_Positions[p1Index].position, scaleFactor);
		return glm::translate(glm::mat4(1.0f), finalPosition);
	}
	glm::mat4 Bone::InterpolateRotation(float animationTime)
	{
		if (1 == m_NumRotations)
		{
			auto rotation = glm::normalize(m_Rotations[0].rotation);
			return glm::toMat4(rotation);
		}

		int r0Index = GetRotationIndex(animationTime);
		int r1Index = r0Index + 1;

		float scaleFactor = GetScaleFactor(m_Rotations[r0Index].timeStamp, m_Rotations[r1Index].timeStamp, animationTime);
		glm::quat finalRotation = glm::slerp(m_Rotations[r0Index].rotation, m_Rotations[r1Index].rotation, scaleFactor);
		finalRotation = glm::normalize(finalRotation);
		return glm::toMat4(finalRotation);
	}
	glm::mat4 Bone::InterpolateScale(float animationTime)
	{
		if (1 == m_NumScales)
			return glm::scale(glm::mat4(1.0f), m_Scale[0].scale);

		int s0Index = GetScaleIndex(animationTime);
		int s1Index = s0Index + 1;

		float scaleFactor = GetScaleFactor(m_Scale[s0Index].timeStamp, m_Scale[s1Index].timeStamp, animationTime);
		glm::vec3 finalScale = glm::mix(m_Scale[s0Index].scale, m_Scale[s1Index].scale, scaleFactor);
		return glm::scale(glm::mat4(1.0f), finalScale);
	}
	glm::vec3 Bone::GetGLMVec3(const aiVector3D& vector)
	{
		return glm::vec3(vector.x, vector.y, vector.z);
	}
	glm::quat Bone::GetGLMQuat(const aiQuaternion& quaternion)
	{
		return glm::quat(quaternion.w, quaternion.x, quaternion.y, quaternion.z);
	}
}