#pragma once
#include <pch.h>
#include <GL/GLCore.h>
#include <Renderer/Model.h>

#include "Utility/Utility.h"

namespace Engine {
  class Scene;

  struct FolderPaths {
    std::string assets;
    std::string fonts;
    std::string materials;
    std::string models;
    std::string resources;
    std::string scenes;
    std::string shaders;
    std::string skyboxes;
    std::string sounds;
    std::string textures;
  };

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
    static inline std::unordered_map< std::string,
                                      std::shared_ptr< std::map< char, Utility::Character > > >
        _loadedCharacters;
    static inline std::unordered_map< std::string, std::shared_ptr< GL::Cubemap > > _loadedCubemaps;
    static FolderPaths _folderPahts;

  public:
    static auto GetAssetsFolders() -> FolderPaths;
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
    static auto GetCharacters(std::string file, int fontSize)
        -> std::shared_ptr< std::map< char, Utility::Character > >;
    static auto GetCubemap(std::string folder) -> std::shared_ptr< GL::Cubemap >;
  };
}  // namespace Engine