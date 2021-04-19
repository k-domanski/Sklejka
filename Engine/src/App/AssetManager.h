#pragma once
#include <pch.h>
#include <GL/GLCore.h>
#include <Renderer/Model.h>

namespace Engine {
  class AssetManager {
  private:
    static inline std::unordered_map< std::string_view, std::shared_ptr< GL::Shader > >
        _loadedShaders;
    static inline std::unordered_map< std::string_view, std::shared_ptr< Renderer::Model > >
        _loadedModels;
    static inline std::unordered_map< Renderer::ModelPrimitive::_integral,
                                      std::shared_ptr< Renderer::Model > >
        _primitiveModels;
    static inline std::unordered_map< std::string_view, std::shared_ptr< GL::Texture2D > >
        _loadedTextures2D;
    static inline std::unordered_map< std::size_t, std::shared_ptr< Renderer::Material > >
        _loadedMaterials;

    static auto generateID();

  public:
    static auto GetShader(const std::string_view& file) -> std::shared_ptr< GL::Shader >;
    static auto GetModel(const std::string_view& file) -> std::shared_ptr< Renderer::Model >;
    static auto GetModel(Renderer::ModelPrimitive primitive) -> std::shared_ptr< Renderer::Model >;
    static auto GetTexture2D(const std::string_view& file) -> std::shared_ptr< GL::Texture2D >;
    static auto GetMaterial(std::shared_ptr< GL::Shader > shared_ptr,
                            std::shared_ptr< GL::Texture2D > texture_2d)
        -> std::shared_ptr< Renderer::Material >;
    static auto GetMaterial(std::shared_ptr< GL::Shader > shared_ptr,
                            std::shared_ptr< GL::Texture2D > texture_2d,
                            std::size_t assetID)
        -> std::shared_ptr< Renderer::Material >;
  };
}  // namespace Engine