#include "pch.h"
#include "AssetManager.h"
#include "Renderer/Material.h"

#include <random>
#include <Utility/Utility.h>
#include <App/Log.h>

namespace Engine {
  auto AssetManager::generateID() {
    std::random_device dv;
    std::mt19937_64 mt(dv());
    return static_cast< size_t >(mt());
  }

  auto AssetManager::GetShader(const std::string_view& file) -> std::shared_ptr< GL::Shader > {
    if (_loadedShaders.count(file) == 0) {
      auto shaderSource = Utility::ReadTextFile(file);
      auto parseResult  = Utility::ParseShaderSource(shaderSource, std::string(file));
      if (!parseResult.success) {
        CORE_ERROR("Shader parsing error: {0}", parseResult.infoMessage);
        return nullptr;
      }
      auto shader = std::make_shared< GL::Shader >();
      shader->FilePath(file);
      for (auto& sh : parseResult.shaders) {
        auto subShader = std::make_shared< GL::SubShader >(sh.first, sh.second);
        shader->AttachShader(subShader);
      }
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
  auto AssetManager::GetModel(Renderer::ModelPrimitive primitive)
      -> std::shared_ptr< Renderer::Model > {
    if (_primitiveModels.count(primitive) == 0) {
      _primitiveModels[primitive] =
          std::make_shared< Renderer::Model >(Renderer::Mesh::GetPrimitive(primitive));
    }
    return _primitiveModels[primitive];
  }
  auto AssetManager::GetTexture2D(const std::string_view& file)
      -> std::shared_ptr< GL::Texture2D > {
    if (_loadedTextures2D.count(file) == 0) {
      int x, y, n;
      auto pixel_data = stbi_load(file.data(), &x, &y, &n, 4);
      auto texture    = std::make_shared< GL::Texture2D >(x, y, pixel_data);
      texture->FilePath(file);
      stbi_image_free(pixel_data);
      _loadedTextures2D[file] = texture;
      return texture;
    }
    return _loadedTextures2D[file];
  }

  auto AssetManager::GetMaterial(std::shared_ptr< GL::Shader > shared_ptr,
                                 std::shared_ptr< GL::Texture2D > texture_2d)
      -> std::shared_ptr< Renderer::Material > {
    return AssetManager::GetMaterial(shared_ptr, texture_2d, generateID());
  }

  auto AssetManager::GetMaterial(std::shared_ptr< GL::Shader > shared_ptr,
                                 std::shared_ptr< GL::Texture2D > texture_2d, std::size_t assetID)
      -> std::shared_ptr< Renderer::Material > {
    for (auto [key, loaded_material] : _loadedMaterials) {
      if (loaded_material->GetShader() == shared_ptr
          && loaded_material->GetDiffuse() == texture_2d) {
        return loaded_material;
      }
    }

    std::shared_ptr< Renderer::Material > newMaterial =
        std::make_shared< Renderer::Material >(assetID);

    if (shared_ptr != nullptr) {
      newMaterial->SetShader(shared_ptr, std::string(shared_ptr->FilePath()));
    }
    if (texture_2d != nullptr) {
      newMaterial->SetMainTexture(texture_2d, std::string(texture_2d->FilePath()));
    }
    _loadedMaterials[newMaterial->GetAssetID()] = newMaterial;
    return newMaterial;
  }
}  // namespace Engine
