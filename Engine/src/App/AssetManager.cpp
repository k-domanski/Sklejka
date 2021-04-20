#include "pch.h"
#include "AssetManager.h"
#include "Renderer/Material.h"

#include <random>
#include <Utility/Utility.h>
#include <App/Log.h>
#include <filesystem>

namespace Engine {
  auto AssetManager::GenerateAssetID() {
    std::random_device dv;
    std::mt19937_64 mt(dv());
    return static_cast< size_t >(mt());
  }

  auto AssetManager::GetWoringDir() -> std::string {
    return std::filesystem::current_path().string();
  }

  auto AssetManager::GetShader(const std::string& file) -> std::shared_ptr< GL::Shader > {
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
  auto AssetManager::GetModel(const std::string& file) -> std::shared_ptr< Renderer::Model > {
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
  auto AssetManager::GetTexture2D(const std::string& file) -> std::shared_ptr< GL::Texture2D > {
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
  auto AssetManager::CreateMaterial() -> std::shared_ptr< Renderer::Material > {
    auto assetID              = GenerateAssetID();
    auto material             = std::make_shared< Renderer::Material >(assetID);
    _loadedMaterials[assetID] = material;
    return material;
  }
  auto AssetManager::CreateMaterial(const std::string& filePath)
      -> std::shared_ptr< Renderer::Material > {
    auto assetID              = GenerateAssetID();
    auto material             = std::make_shared< Renderer::Material >(assetID);
    _loadedMaterials[assetID] = material;
    material->FilePath(filePath);
    return material;
  }
  auto AssetManager::GetMaterial(const std::string& file) -> std::shared_ptr< Renderer::Material > {
    /* Check if material from this file is already loaded */
    auto it = std::find_if(_loadedMaterials.begin(), _loadedMaterials.end(),
                           [file](const auto& kv) { return kv.second->FilePath() == file; });
    /* If not loaded, load and return */
    if (it == _loadedMaterials.end()) {
      auto content        = Utility::ReadTextFile(file);
      nlohmann::json json = nlohmann::json::parse(content.begin(), content.end());

      std::string assetID_string = json["assetID"];
      std::stringstream ss(assetID_string);
      size_t assetID;
      ss >> assetID;

      std::shared_ptr< GL::Shader > shader_ptr     = GetShader(json["shaderFilepath"]);
      std::shared_ptr< GL::Texture2D > texture_ptr = GetTexture2D(json["diffuseFilepath"]);
      auto material = std::make_shared< Renderer::Material >(assetID);
      material->SetShader(shader_ptr);
      material->SetMainTexture(texture_ptr);
      _loadedMaterials[assetID] = material;
      return material;
    }
    return it->second;
  }
  auto AssetManager::GetMaterial(std::size_t assetID) -> std::shared_ptr< Renderer::Material > {
    if (_loadedMaterials.count(assetID) == 0) {
      LOG_WARN("No material with id {} loaded", assetID);
      return nullptr;
    }
    return _loadedMaterials[assetID];
  }
}  // namespace Engine
