#pragma once
#include <pch.h>
#include "assimp/scene.h"
#include <Renderer/Mesh.h>

namespace Engine::Renderer {
  typedef MeshPrimitive ModelPrimitive;

  class Material;
  class Model {
  public:
    Model() = default;
    Model(const std::shared_ptr< Mesh >& mesh) noexcept;
    Model(std::string_view path);
    virtual ~Model() = default;

    std::shared_ptr< Mesh > getRootMesh();

  private:
    std::vector< std::shared_ptr< Mesh > > meshes;
    void loadModel(std::string_view path);
    void processNode(aiNode* node, const aiScene* scene, std::shared_ptr< Mesh > parent = nullptr);
    std::shared_ptr< Mesh > processMesh(aiMesh* mesh, const aiScene* scene);
  };
}  // namespace Engine::Renderer
