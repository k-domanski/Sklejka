#pragma once
#include "ECS/ECS.h"
#include "Renderer/Animation/Animation.h"
#include "Renderer/Model.h"
#include "GL/UniformBufferData.h"

namespace Engine {
  class Animator : public ECS::Component {
  public:
    Animator();
    Animator(std::shared_ptr< Animation > animation);
    Animator(std::shared_ptr< Renderer::Model > model);
    void UpdateAnimation(float deltaTime);
    void PlayAnimation(std::shared_ptr< Animation > animation);
    void CalculateBoneTransform(const AssimpNodeData& node, glm::mat4 parentTransform);
    /*std::vector< glm::mat4 > GetFinalBoneMatrices() {
      return m_FinalBoneMatrices;
    }*/
    virtual std::string SaveToJson() override;
    virtual void LoadFromJson(std::string path) override;
    std::shared_ptr< Animation > GetAnimation() {
      return m_CurrentAnimation;
    }
    void SetAnimation(std::shared_ptr< Renderer::Model > model);
    //auto SendUniformData() -> void;
    auto GetJointData() -> GL::JointUniformData;

  private:
    auto Init() -> void;

  private:
    GL::JointUniformData m_JointUniformData;
    //GL::JointsUniformBuffer m_JointBuffer;
    // std::vector< glm::mat4 > m_FinalBoneMatrices;
    std::shared_ptr< Animation > m_CurrentAnimation;
    std::map< std::string, Renderer::BoneInfo > m_BoneInfoMap;
    float m_CurrentTime;
    float m_DeltaTime;
    float m_AnimationSpeed = 5.0f;
  };
}  // namespace Engine