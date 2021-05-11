#include "pch.h"
#include "Renderer/Model.h"
#include "Renderer/Mesh.h"

#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "Renderer/Vertex.h"

namespace Engine::Renderer {
  Model::Model(const std::shared_ptr< Mesh >& mesh) noexcept {
    _filepath = "";
    meshes.push_back(mesh);
    CreateBoundingSphere();
    CreateBoundingBox();
    _query = GL::QueryObject();
  }
  Model::Model(std::string_view path) {
    loadModel(path);
    CreateBoundingSphere();
    CreateBoundingBox();
    _query = GL::QueryObject();
  }

  std::shared_ptr< Mesh > Model::GetRootMesh() {
    return meshes[0];
  }

  std::string Model::GetFilepath() {
    return _filepath;
  }

  std::pair< glm::vec3, float > Model::GetBoundingSphere() {
    return std::make_pair(_sphereCenter, _radius);
  }

  std::shared_ptr< Mesh > Model::GetBoundingBox() {
    return _boundingBox;
  }

  auto Model::GetQuery() -> GL::QueryObject& {
    return _query;
  }

  int Model::GetMeshCount()
  {
    return meshes.size();
  }

  std::shared_ptr< Mesh > Model::GetMesh(int index)
  {
    return meshes[index];
  }


  void Model::loadModel(std::string_view path) {
    Assimp::Importer importer;
    const aiScene* scene =
        importer.ReadFile(path.data(), aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
      CORE_ERROR("Failed to load model: {0}", importer.GetErrorString());
      return;
    }

    _filepath = path;
    meshes.reserve(scene->mNumMeshes);
    processNode(scene->mRootNode, scene);
  }

  void Model::processNode(aiNode* node, const aiScene* scene, std::shared_ptr< Mesh > parent) {
    std::shared_ptr< Mesh > lastMesh = nullptr;
    for (size_t i = 0; i < node->mNumMeshes; i++) {
      aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
      meshes.push_back(processMesh(mesh, scene, aiMat4ToGlmMat4(node->mTransformation)));

      lastMesh = meshes.back();
      std::cout << "\nPushing " << mesh->mName.C_Str() << " mesh to meshes vector";
    }

    // Repeat process for all the children
    for (size_t i = 0; i < node->mNumChildren; i++) {
      processNode(node->mChildren[i], scene, lastMesh);
    }
  }

  void Model::CreateBoundingSphere() {
    const auto mesh = GetRootMesh();

    _sphereCenter     = glm::vec3(0.0f);  // for now its enough latter maybe average vertex?
    float maxDistance = 0;

    for (auto vertex : mesh->GetVertices()) {
      _sphereCenter += vertex.position;
    }
    _sphereCenter /= mesh->GetVertices().size();

    for (auto vertex : mesh->GetVertices()) {
      float distance = glm::distance(vertex.position, _sphereCenter);
      if (distance > maxDistance)
        maxDistance = distance;
    }

    _radius = maxDistance;
  }

  void Model::CreateBoundingBox() {
    const auto mesh = GetRootMesh();

    glm::vec3 min(INFINITY);
    glm::vec3 max(-INFINITY);

    for (auto& vertex : mesh->GetVertices()) {
      if (vertex.position.x > max.x)
        max.x = vertex.position.x;
      if (vertex.position.y > max.y)
        max.y = vertex.position.y;
      if (vertex.position.z > max.z)
        max.z = vertex.position.z;
      if (vertex.position.x < min.x)
        min.x = vertex.position.x;
      if (vertex.position.y < min.y)
        min.y = vertex.position.y;
      if (vertex.position.z < min.z)
        min.z = vertex.position.z;
    }

    const std::vector< Vertex > verts{{// Vert 0
                                       {min.x, min.y, min.z},
                                       {0.0f, 0.0f, 0.0f},
                                       {0.0f, 0.0f}},
                                      {// Vert 1
                                       {min.x, max.y, min.z},
                                       {0.0f, 0.0f, 0.0f},
                                       {0.0f, 0.0f}},
                                      {// Vert 2
                                       {min.x, max.y, max.z},
                                       {0.0f, 0.0f, 0.0f},
                                       {0.0f, 0.0f}},
                                      {// Vert 3
                                       {min.x, min.y, max.z},
                                       {0.0f, 0.0f, 0.0f},
                                       {0.0f, 0.0f}},
                                      {// Vert 4
                                       {max.x, min.y, min.z},
                                       {0.0f, 0.0f, 0.0f},
                                       {0.0f, 0.0f}},
                                      {// Vert 5
                                       {max.x, max.y, min.z},
                                       {0.0f, 0.0f, 0.0f},
                                       {0.0f, 0.0f}},
                                      {// Vert 6
                                       {max.x, max.y, max.z},
                                       {0.0f, 0.0f, 0.0f},
                                       {0.0f, 0.0f}},
                                      {// Vert 7
                                       {max.x, min.y, max.z},
                                       {0.0f, 0.0f, 0.0f},
                                       {0.0f, 0.0f}}};
    const std::vector< GLuint > inds{0, 3, 2, 2, 1, 0, 3, 7, 6, 6, 2, 3, 7, 4, 5, 5, 6, 7,
                                     4, 0, 1, 1, 5, 4, 1, 2, 6, 6, 5, 1, 4, 7, 3, 3, 0, 4};
    _boundingBox = std::make_shared< Mesh >(verts, inds);
  }

  std::shared_ptr< Mesh > Model::processMesh(aiMesh* mesh, const aiScene* scene, glm::mat4 transformation) {
    std::vector< Vertex > vertices;
    std::vector< GLuint > indices;

    for (size_t i = 0; i < mesh->mNumVertices; i++) {
      Vertex vertex;
      glm::vec3 vec;
      glm::vec2 uv;

      vec.x           = mesh->mVertices[i].x;
      vec.y           = mesh->mVertices[i].y;
      vec.z           = mesh->mVertices[i].z;
      vertex.position = vec;

      vec.x         = mesh->mNormals[i].x;
      vec.y         = mesh->mNormals[i].y;
      vec.z         = mesh->mNormals[i].z;
      vertex.normal = vec;

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
    }

    for (size_t i = 0; i < mesh->mNumFaces; i++) {
      aiFace face = mesh->mFaces[i];
      for (size_t j = 0; j < face.mNumIndices; j++) {
        indices.push_back(face.mIndices[j]);
      }
    }
    std::shared_ptr<Mesh> final_mesh = std::make_shared< Mesh >(vertices, indices);
    final_mesh->SetName(mesh->mName.C_Str());
    final_mesh->SetModelMatrix(transformation);
    return final_mesh;
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
