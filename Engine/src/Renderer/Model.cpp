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
  }
  Model::Model(std::string_view path) {
    loadModel(path);
    CreateBoundingSphere();
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
      meshes.push_back(processMesh(mesh, scene));

      lastMesh = meshes.back();
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
      float distance = glm::distance(vertex.position, _sphereCenter);
      if (distance > maxDistance)
        maxDistance = distance;
    }

    _radius = maxDistance;
  }

  std::shared_ptr< Mesh > Model::processMesh(aiMesh* mesh, const aiScene* scene) {
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

      uv.x      = mesh->mTextureCoords[0][i].x;
      uv.y      = mesh->mTextureCoords[0][i].y;
      vertex.uv = uv;

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

    return std::make_shared< Mesh >(vertices, indices);
  }

}  // namespace Engine::Renderer
