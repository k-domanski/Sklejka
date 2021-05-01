#pragma once
#include <pch.h>
#include <GL/GLCore.h>
#include <Renderer/Model.h>

namespace Engine {
  class Scene;

  class AssetManager {
  private:
    static inline std::unordered_map< std::string, std::shared_ptr< GL::Shader > > _loadedShaders;
    static inline std::unordered_map< std::string, std::shared_ptr< Renderer::Model > >
        _loadedModels;
    static inline std::unordered_map< Renderer::ModelPrimitive::_integral,
                                      std::shared_ptr< Renderer::Model > >
        _primitiveModels;
    static inline std::unordered_map< std::string, std::shared_ptr< GL::Texture2D > >
        _loadedTextures2D;
    static inline std::unordered_map< std::size_t, std::shared_ptr< Renderer::Material > >
        _loadedMaterials;
    static inline std::unordered_map< std::string, std::shared_ptr< Scene > > _loadedScenes;

  public:
    static auto GenerateAssetID() -> size_t;
    static auto GetWoringDir() -> std::string;
    static auto GetShader(std::string file) -> std::shared_ptr< GL::Shader >;
    static auto GetModel(std::string file) -> std::shared_ptr< Renderer::Model >;
    static auto GetModel(Renderer::ModelPrimitive primitive) -> std::shared_ptr< Renderer::Model >;
    static auto GetTexture2D(std::string file) -> std::shared_ptr< GL::Texture2D >;
    static auto CreateMaterial() -> std::shared_ptr< Renderer::Material >;
    static auto CreateMaterial(std::string filePath) -> std::shared_ptr< Renderer::Material >;
    static auto GetMaterial(std::string file) -> std::shared_ptr< Renderer::Material >;
    static auto GetMaterial(std::size_t assetID) -> std::shared_ptr< Renderer::Material >;
    static auto SaveScene(const std::shared_ptr< Scene >& scene, std::string file) -> void;
    static auto LoadScene(std::string file) -> std::shared_ptr< Scene >;
  };
}  // namespace Engine