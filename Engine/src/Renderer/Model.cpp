#include "pch.h"
#include "Renderer/Model.h"
#include "Renderer/Mesh.h"

#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "Renderer/Vertex.h"

namespace Engine::Renderer {
  Model::Model(const std::shared_ptr< Mesh >& mesh) noexcept {
    _filepath = "";
    mesh->CreateBoundingBox();
    mesh->CreateBoundingSphere();
    _meshes.push_back(mesh);
  }
  Model::Model(std::string_view path) {
    LoadModel(path);
  }

  std::shared_ptr< Mesh > Model::GetRootMesh() {
    if (_meshes.size() > 0) {
      return _meshes[0];
    }
    return nullptr;
  }

  std::string Model::GetFilepath() {
    return _filepath;
  }

  int Model::GetMeshCount() {
    return _meshes.size();
  }

  int Model::GetNodeCount()
  {
    return nodes.size();
  }

  aiNode* Model::GetNode(int index) {
    return nodes[index];
  }

  std::shared_ptr< Mesh > Model::GetMesh(int index) {
    return _meshes[glm::clamp(index, 0, static_cast< int >(_meshes.size()) - 1)];
  }

  auto Model::GetMesh(int node, int index) -> std::shared_ptr< Mesh > {
    if (node >= _nodes.size() || index >= _nodes[node].mesh_indexes.size()) {
      return nullptr;
    }
    return _meshes[_nodes[node].mesh_indexes[index]];
  }

  void Model::loadModel(std::string_view path) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(
        path.data(),
        aiProcess_Triangulate /*| aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace*/
        /*| aiProcess_FlipUVs*/);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
      CORE_ERROR("Failed to load model: {0}", importer.GetErrorString());
      return;
    }

    _filepath = path;
    // meshes.reserve(scene->mNumMeshes);
    int nodeCount = countNodes(scene->mRootNode);
    LOG_DEBUG("Node count is {}", nodeCount);
    nodes.reserve(nodeCount);
    processNode(scene->mRootNode, scene);
  }

  void Model::processNode(aiNode* node, const aiScene* scene, std::shared_ptr< Mesh > parent) {
    std::shared_ptr< Mesh > lastMesh = nullptr;
    LOG_DEBUG("Loading node {}", node->mName.C_Str());
    nodes.push_back(node);

    for (size_t i = 0; i < node->mNumMeshes; i++) {
      aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
      /*int parentIndex =
          parent != nullptr ? find(meshes.begin(), meshes.end(), parent) - meshes.begin() : -1;*/
      /*meshes.push_back(
          processMesh(mesh, scene, aiMat4ToGlmMat4(node->mTransformation), parentIndex));*/

      /*lastMesh = meshes.back();
      LOG_DEBUG("Pushing {} mesh to meshes vector with meshParent {}", mesh->mName.C_Str(),
                parentIndex);*/
    }

    // Repeat process for all the children
    for (size_t i = 0; i < node->mNumChildren; i++) {
      processNode(node->mChildren[i], scene, lastMesh);
    }
  }

  int Model::countNodes(aiNode* node)
  {
    int res = 1;
    for (int i = 0; i < node->mNumChildren; i++)
    {
      res += countNodes(node->mChildren[i]);
    }

    return res;
  }

 

  std::shared_ptr< Mesh > Model::processMesh(aiMesh* mesh, const aiScene* scene,
                                             glm::mat4 transformation, int parentIndex) {
    std::vector< Vertex > vertices;
    std::vector< GLuint > indices;
    std::vector< JointVertexData > joints;
    m_NumberOfBones = 0;
    
    /*--------------Animation--------------------*/
    if (mesh->HasBones()) {
      for (int i = 0; i < mesh->mNumBones; i++) {
        joints.resize(mesh->mNumVertices);
        unsigned int BoneIndex = 0;
        std::string BoneName(mesh->mBones[i]->mName.data);
        if (m_BoneMap.find(BoneName) == m_BoneMap.end()) {
          BoneIndex = m_NumberOfBones;
          m_NumberOfBones++;
          m_BoneMap[BoneName].inverseBindPose = aiMat4ToGlmMat4(mesh->mBones[i]->mOffsetMatrix);
        } else {
          BoneIndex = m_BoneMap[BoneName].boneId;
        }

        m_BoneMap[BoneName].boneId = BoneIndex;
        for (int j = 0; j < mesh->mBones[i]->mNumWeights; j++) {
          unsigned int id = mesh->mBones[i]->mWeights[j].mVertexId;
          float weight    = mesh->mBones[i]->mWeights[j].mWeight;
          joints[id].AddJointData(BoneIndex, weight);
        }
      }
    }
    /*------------------------------------------*/
    for (size_t i = 0; i < mesh->mNumVertices; i++) {
      Vertex vertex;
      glm::vec3 vec;
      glm::vec2 uv;
      glm::ivec4 boneID;
      glm::vec4 weights;
      vec.x           = mesh->mVertices[i].x;
      vec.y           = mesh->mVertices[i].y;
      vec.z           = mesh->mVertices[i].z;
      vertex.position = vec;

      vec.x         = mesh->mNormals[i].x;
      vec.y         = mesh->mNormals[i].y;
      vec.z         = mesh->mNormals[i].z;
      vertex.normal = vec;

      if (mesh->HasBones())
      {
          boneID.x = joints[i].BoneIDs[0];
          boneID.y = joints[i].BoneIDs[1];
          boneID.z = joints[i].BoneIDs[2];
          boneID.w = joints[i].BoneIDs[3];
          vertex.jointIDs = boneID;

          weights.x = joints[i].weights[0];
          weights.y = joints[i].weights[1];
          weights.z = joints[i].weights[2];
          weights.w = joints[i].weights[3];
          vertex.weights = weights;
      }
      else {
          boneID = glm::vec4(0);
          vertex.jointIDs = boneID;

          weights = glm::vec4(0.0f);
          vertex.weights = weights;
      }

      /* uv.x      = mesh->mTextureCoords[0][i].x;
       uv.y      = mesh->mTextureCoords[0][i].y;
       vertex.uv = uv;*/
      if (mesh->mTextureCoords[0]) {
        glm::vec2 vec;
        vec.x     = mesh->mTextureCoords[0][i].x;
        vec.y     = mesh->mTextureCoords[0][i].y;
        vertex.uv = vec;
      } else {
        vertex.uv = glm::vec2(0.0f, 0.0f);
      }

      // Add the created vertex to the vector
      vertices.push_back(vertex);
      if (vertex.jointIDs[0] > 51 || vertex.jointIDs[1] > 51 || vertex.jointIDs[2] > 51
          || vertex.jointIDs[3] > 51)
        CORE_DEBUG("[{}][{}][{}][{}]", vertex.jointIDs[0], vertex.jointIDs[1], vertex.jointIDs[2],
                   vertex.jointIDs[3]);
    }

    for (size_t i = 0; i < mesh->mNumFaces; i++) {
      aiFace face = mesh->mFaces[i];
      for (size_t j = 0; j < face.mNumIndices; j++) {
        indices.push_back(face.mIndices[j]);
      }
    }
    std::shared_ptr< Mesh > final_mesh = std::make_shared< Mesh >(vertices, indices);
    final_mesh->SetName(mesh->mName.C_Str());
    final_mesh->SetModelMatrix(transformation);
    final_mesh->SetParentMesh(parentIndex);
    return final_mesh;
  }

  auto Model::LoadModel(std::string_view path) -> void {
    Assimp::Importer importer;
    const aiScene* scene =
        importer.ReadFile(path.data(), aiProcess_Triangulate | aiProcess_GenSmoothNormals
                                           | aiProcess_CalcTangentSpace | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
      CORE_ERROR("Failed to load model: {0}", importer.GetErrorString());
      return;
    }

    _filepath = path;

    _meshes.resize(scene->mNumMeshes);
    for (int i = 0; i < scene->mNumMeshes; ++i) {
      // Load mesh and place it in the `_meshes`
      _meshes[i] = LoadMesh(scene->mMeshes[i]);
      _meshes[i]->CreateBoundingBox();
      _meshes[i]->CreateBoundingSphere();
    }
    // Load hierarchy and transforms
    ProcessNode(scene->mRootNode, scene, -1);
  }

  auto Model::LoadMesh(aiMesh* mesh) -> std::shared_ptr< Mesh > {
    std::vector< Vertex > vertices;
    std::vector< GLuint > indices;
    std::vector< JointVertexData > joints;
    m_NumberOfBones = 0;

    vertices.reserve(mesh->mNumVertices);
    indices.reserve(mesh->mNumFaces * 3);
    joints.resize(mesh->mNumVertices);
    /*--------------Animation--------------------*/
    if (mesh->HasBones()) {
      for (int i = 0; i < mesh->mNumBones; i++) {
        unsigned int BoneIndex = 0;
        std::string BoneName(mesh->mBones[i]->mName.data);
        if (m_BoneMap.find(BoneName) == m_BoneMap.end()) {
          BoneIndex = m_NumberOfBones;
          m_NumberOfBones++;
          m_BoneMap[BoneName].inverseBindPose = aiMat4ToGlmMat4(mesh->mBones[i]->mOffsetMatrix);
        } else {
          BoneIndex = m_BoneMap[BoneName].boneId;
        }

        m_BoneMap[BoneName].boneId = BoneIndex;
        for (int j = 0; j < mesh->mBones[i]->mNumWeights; j++) {
          unsigned int id = mesh->mBones[i]->mWeights[j].mVertexId;
          float weight    = mesh->mBones[i]->mWeights[j].mWeight;
          joints[id].AddJointData(BoneIndex, weight);
        }
      }
    }
    /*------------------------------------------*/

    /*-------------Vertex Data------------------*/
    for (size_t i = 0; i < mesh->mNumVertices; i++) {
      Vertex vertex;
      glm::vec3 vec;
      glm::vec2 uv;
      glm::ivec4 boneID;
      glm::vec4 weights;
      vec.x           = mesh->mVertices[i].x;
      vec.y           = mesh->mVertices[i].y;
      vec.z           = mesh->mVertices[i].z;
      vertex.position = vec;

      vec.x         = mesh->mNormals[i].x;
      vec.y         = mesh->mNormals[i].y;
      vec.z         = mesh->mNormals[i].z;
      vertex.normal = vec;

      boneID.x        = joints[i].BoneIDs[0];
      boneID.y        = joints[i].BoneIDs[1];
      boneID.z        = joints[i].BoneIDs[2];
      boneID.w        = joints[i].BoneIDs[3];
      vertex.jointIDs = boneID;

      weights.x      = joints[i].weights[0];
      weights.y      = joints[i].weights[1];
      weights.z      = joints[i].weights[2];
      weights.w      = joints[i].weights[3];
      vertex.weights = weights;

      if (mesh->mTextureCoords[0]) {
        glm::vec2 vec;
        vec.x     = mesh->mTextureCoords[0][i].x;
        vec.y     = mesh->mTextureCoords[0][i].y;
        vertex.uv = vec;
      } else {
        vertex.uv = glm::vec2(0.0f, 0.0f);
      }

      // Add the created vertex to the vector
      vertices.push_back(vertex);
    }

    for (size_t i = 0; i < mesh->mNumFaces; i++) {
      aiFace face = mesh->mFaces[i];
      for (size_t j = 0; j < face.mNumIndices; j++) {
        indices.push_back(face.mIndices[j]);
      }
    }
    /*------------------------------------------*/
    std::shared_ptr< Mesh > final_mesh = std::make_shared< Mesh >(vertices, indices);
    final_mesh->SetName(mesh->mName.C_Str());
    return final_mesh;
  }

  auto Model::ProcessNode(aiNode* node, const aiScene* scene, int parent_node) -> void {
    Node s_node;
    s_node.parent_node = parent_node;
    s_node.name        = node->mName.data;
    //if (s_node.name == "skalka pionowa 1.159")
    //    __debugbreak();
    /* Local transform */
    s_node.transform = aiMat4ToGlmMat4(node->mTransformation);

    for (size_t i = 0; i < node->mNumMeshes; i++) {
      s_node.mesh_indexes.push_back(node->mMeshes[i]);
    }
    _nodes.push_back(s_node);
    auto last_index = _nodes.size() - 1;
    for (size_t i = 0; i < node->mNumChildren; i++) {
      ProcessNode(node->mChildren[i], scene, last_index);
    }
  }

  auto Model::Nodes() const -> const std::vector< Node >& {
    return _nodes;
  }

  glm::mat4 Model::aiMat4ToGlmMat4(aiMatrix4x4 aiMat) {
    glm::mat4 output;
    output[0][0] = aiMat[0][0];
    output[0][1] = aiMat[1][0];
    output[0][2] = aiMat[2][0];
    output[0][3] = aiMat[3][0];

    output[1][0] = aiMat[0][1];
    output[1][1] = aiMat[1][1];
    output[1][2] = aiMat[2][1];
    output[1][3] = aiMat[3][1];

    output[2][0] = aiMat[0][2];
    output[2][1] = aiMat[1][2];
    output[2][2] = aiMat[2][2];
    output[2][3] = aiMat[3][2];

    output[3][0] = aiMat[0][3];
    output[3][1] = aiMat[1][3];
    output[3][2] = aiMat[2][3];
    output[3][3] = aiMat[3][3];

    return output;
  }

}  // namespace Engine::Renderer
