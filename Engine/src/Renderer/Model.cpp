#include "pch.h"
#include "Renderer/Model.h"
#include "Renderer/Mesh.h"

#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "Renderer/Vertex.h"

namespace Engine::Renderer {
  Model::Model(std::string path) {
    loadModel(path);
  }

  Mesh* Model::getRootMesh() {
    return &meshes[0];
  }

  void Model::loadModel(std::string path) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
      std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
      return;
    }

    meshes.reserve(scene->mNumMeshes);
    processNode(scene->mRootNode, scene);
  }

  void Model::processNode(aiNode* node, const aiScene* scene, Mesh* parent) {
    Mesh* lastMesh = nullptr;
    for (size_t i = 0; i < node->mNumMeshes; i++) {
      aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
      meshes.push_back(processMesh(mesh, scene));

      lastMesh = &meshes.back();
    }

    // Repeat process for all the children
    for (size_t i = 0; i < node->mNumChildren; i++) {
      processNode(node->mChildren[i], scene, lastMesh);
    }
  }

  Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene) {
    std::vector< Vertex > vertices;
    std::vector< GLuint > indices;

    for (size_t i = 0; i < mesh->mNumVertices; i++) {
      Vertex vertex;
      glm::vec3 vec;

      vec.x           = mesh->mVertices[i].x;
      vec.y           = mesh->mVertices[i].y;
      vec.z           = mesh->mVertices[i].z;
      vertex.position = vec;

      vec.x         = mesh->mNormals[i].x;
      vec.y         = mesh->mNormals[i].y;
      vec.z         = mesh->mNormals[i].z;
      vertex.normal = vec;

      // Add the created vertex to the vector
      vertices.push_back(vertex);
    }

    for (size_t i = 0; i < mesh->mNumFaces; i++) {
      aiFace face = mesh->mFaces[i];
      for (size_t j = 0; j < face.mNumIndices; j++) {
        indices.push_back(face.mIndices[j]);
      }
    }

    return Mesh(vertices, indices);
  }

}  // namespace Engine::Renderer
