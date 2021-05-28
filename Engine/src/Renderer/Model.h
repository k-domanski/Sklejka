#pragma once
#include <pch.h>
#include "assimp/scene.h"
#include <Renderer/Mesh.h>

namespace Engine::Renderer {
  typedef MeshPrimitive ModelPrimitive;
#define NUM_JOINTS_PER_VERTEX 4
  class Material;
  struct BoneInfo {
      unsigned int boneId;
      glm::mat4 inverseBindPose;
  };
  class Model {
  public:
    Model() = default;
    Model(const std::shared_ptr< Mesh >& mesh) noexcept;
    Model(std::string_view path);
    virtual ~Model() = default;

    std::shared_ptr< Mesh > GetRootMesh();
    std::string GetFilepath();
    std::pair< glm::vec3, float > GetBoundingSphere();
    std::shared_ptr<Mesh> GetBoundingBox();
    auto GetQuery() -> GL::QueryObject&;
    int GetMeshCount();
    int GetNodeCount();
    aiNode* GetNode(int index);
    std::shared_ptr< Mesh > GetMesh(int index);
    static glm::mat4 aiMat4ToGlmMat4(aiMatrix4x4 aiMat);
    std::map<std::string, BoneInfo> GetBoneInfoMap() { return m_BoneMap; }
    unsigned int GetBoneCount() { return m_NumberOfBones; }

  private:
    std::vector< std::shared_ptr< Mesh > > meshes;
    std::vector< aiNode* > nodes;
    void loadModel(std::string_view path);
    void processNode(aiNode* node, const aiScene* scene, std::shared_ptr< Mesh > parent = nullptr);
    int countNodes(aiNode* node);
    void CreateBoundingSphere();
    void CreateBoundingBox();
    std::shared_ptr< Mesh > processMesh(aiMesh* mesh, const aiScene* scene,
                                        glm::mat4 transformation, int parentIndex);
    std::string _filepath;
    glm::vec3 _sphereCenter;
    float _radius;
    GL::QueryObject _query;
    std::shared_ptr< Mesh > _boundingBox;

    /*Animation*/
    struct JointVertexData {
        std::string name;
        unsigned int BoneIDs[NUM_JOINTS_PER_VERTEX] = { 0 };
        float weights[NUM_JOINTS_PER_VERTEX] = { 0.0f };

        void AddJointData(unsigned int BoneID, float weight)
        {
            for (int i = 0; i < NUM_JOINTS_PER_VERTEX; i++)
            {
                if (weights[i] == 0.0f)
                {
                    BoneIDs[i] = BoneID;
                    weights[i] = weight;
                    return;
                }
            }
        }
    };
    
    std::map<std::string, BoneInfo> m_BoneMap;
    unsigned int m_NumberOfBones;
    /*--------------*/
  };
}  // namespace Engine::Renderer
