#pragma once
#include "assimp/scene.h"

namespace Engine::Renderer {
  class Mesh;
  class Material;
  class Model
  {
  public:
    Model(std::string path);
    virtual ~Model() = default;

    Mesh* getRootMesh();
  private:
    std::vector< Mesh > meshes;
    void loadModel(std::string path);
    void processNode(aiNode* node, const aiScene* scene, Mesh* parent = nullptr);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
  };
}  
