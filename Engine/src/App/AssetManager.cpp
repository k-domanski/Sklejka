#include "pch.h"
#include "AssetManager.h"
#include <Utility/Utility.h>
#include <App/Log.h>

namespace Engine {
  auto AssetManager::GetShader(const std::string_view& file) -> std::shared_ptr< GL::Shader > {
    if (_loadedShaders.count(file) == 0) {
      auto shaderSource = Utility::ReadTextFile(file);
      auto parseResult  = Utility::ParseShaderSource(shaderSource);
      if (!parseResult.success) {
        CORE_ERROR("Shader parsing error: {0}", parseResult.infoMessage);
        return nullptr;
      }
      auto vert =
          std::make_shared< GL::SubShader >(GL::ShaderType::VertexShader, parseResult.vertexShader);
      auto frag   = std::make_shared< GL::SubShader >(GL::ShaderType::FragmentShader,
                                                    parseResult.fragmentShader);
      auto shader = std::make_shared< GL::Shader >();
      shader->AttachShader(vert);
      shader->AttachShader(frag);
      if (!shader->Link()) {
        return nullptr;
      }
      _loadedShaders[file] = shader;
      return shader;
    }
    return _loadedShaders[file];
  }
  auto AssetManager::GetModel(const std::string_view& file) -> std::shared_ptr< Renderer::Model > {
    if (_loadedModels.count(file) == 0) {
      auto model          = std::make_shared< Renderer::Model >(file);
      _loadedModels[file] = model;
      return model;
    }
    return _loadedModels[file];
  }
  auto AssetManager::GetTexture2D(const std::string_view& file)
      -> std::shared_ptr< GL::Texture2D > {
    if (_loadedTextures2D.count(file) == 0) {
      int x, y, n;
      auto pixel_data = stbi_load("./textures/pepo_sad.png", &x, &y, &n, 4);
      auto texture    = std::make_shared< GL::Texture2D >(x, y, pixel_data);
      stbi_image_free(pixel_data);
      _loadedTextures2D[file] = texture;
      return texture;
    }
    return _loadedTextures2D[file];
  }
}  // namespace Engine
