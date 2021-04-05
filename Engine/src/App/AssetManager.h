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
    static inline std::unordered_map< std::string_view, std::shared_ptr< GL::Texture2D > >
        _loadedTextures2D;

  public:
    static auto GetShader(const std::string_view& file) -> std::shared_ptr< GL::Shader >;
    static auto GetModel(const std::string_view& file) -> std::shared_ptr< Renderer::Model >;
    static auto GetTexture2D(const std::string_view& file) -> std::shared_ptr< GL::Texture2D >;
  };
}  // namespace Engine