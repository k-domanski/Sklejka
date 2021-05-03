#include "pch.h"
#include "AssetManager.h"
#include "Renderer/Material.h"
#include <Engine/Scene.h>
#include <Systems/SceneGraph.h>
#include <Components/Camera.h>
#include <Systems/CameraSystem.h>

#include <random>
#include <Utility/Utility.h>
#include <App/Log.h>
#include <filesystem>

namespace fs = std::filesystem;
using namespace Engine::Utility;

auto FileExists(const std::string& path) -> bool {
  if (!fs::exists(path)) {
    LOG_WARN("File does not exist: {}", path);
    return false;
  }
  return true;
}

namespace Engine {
  auto AssetManager::GenerateAssetID() -> size_t {
    std::random_device dv;
    std::mt19937_64 mt(dv());
    return static_cast< size_t >(mt());
  }

  auto AssetManager::GetWoringDir() -> std::string {
    return std::filesystem::current_path().string();
  }

  auto AssetManager::GetShader(std::string file) -> std::shared_ptr< GL::Shader > {
    file = StripToRelativePath(file);
    if (_loadedShaders.count(file) == 0) {
      if (!FileExists(file)) {
        return nullptr;
      }
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
  auto AssetManager::GetModel(std::string file) -> std::shared_ptr< Renderer::Model > {
    file = StripToRelativePath(file);
    if (_loadedModels.count(file) == 0) {
      if (!FileExists(file)) {
        return nullptr;
      }
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
  auto AssetManager::GetTexture2D(std::string file) -> std::shared_ptr< GL::Texture2D > {
    file = StripToRelativePath(file);
    if (_loadedTextures2D.count(file) == 0) {
      if (!FileExists(file)) {
        return nullptr;
      }
      int x, y, n;
      auto pixel_data = stbi_loadf(file.data(), &x, &y, &n, 4);
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
  auto AssetManager::CreateMaterial(std::string filePath) -> std::shared_ptr< Renderer::Material > {
    filePath                  = StripToRelativePath(filePath);
    auto assetID              = GenerateAssetID();
    auto material             = std::make_shared< Renderer::Material >(assetID);
    _loadedMaterials[assetID] = material;
    material->FilePath(filePath);
    return material;
  }
  auto AssetManager::GetMaterial(std::string file) -> std::shared_ptr< Renderer::Material > {
    /* Check if material from this file is already loaded */
    file    = StripToRelativePath(file);
    auto it = std::find_if(_loadedMaterials.begin(), _loadedMaterials.end(),
                           [file](const auto& kv) { return kv.second->FilePath() == file; });
    /* If not loaded, load and return */
    if (it == _loadedMaterials.end()) {
      if (!FileExists(file)) {
        return nullptr;
      }
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
      if (json.count("mainColor") > 0) {
        auto vec = json["mainColor"].get< std::vector< float > >();
        material->MainColor({vec[0], vec[1], vec[2], vec[3]});
      }
      _loadedMaterials[assetID] = material;
      material->FilePath(file);
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
  auto AssetManager::SaveScene(const std::shared_ptr< Scene >& scene, std::string file) -> void {
    const std::string separator = "42091169692137SUPERJSONENTITYSEPARATOR42091169692137";
    auto sg                     = scene->SceneGraph();
    auto entities_ids           = sg->GetChildren(0);

    using namespace nlohmann;
    json json = nlohmann::json{
        {"sceneID", SceneManager::GetCurrentScene()->GetID()},
    };

    std::string fileContent = json.dump(4);

    for (auto id : entities_ids) {
      const auto& entity = ECS::EntityManager::GetInstance().GetEntity(id);
      if (entity == nullptr) {
        continue;
      }
      // Ommit serialization of editor camera
      if (const auto& camera_component = entity->GetComponent< Camera >();
          camera_component != nullptr) {
        if (camera_component->flags.Get(CameraFlag::EditorCamera)) {
          continue;
        }
      }
      fileContent.append("\n" + separator + "\n");

      auto entity_json = entity->SaveToJson();

      fileContent.append(entity_json);
    }

    std::ofstream ofstream;
    ofstream.open(file);
    ofstream << fileContent;
    ofstream.close();
  }
  auto AssetManager::LoadScene(std::string file) -> std::shared_ptr< Scene > {
    file = Utility::StripToRelativePath(file);
    if (_loadedScenes.count(file) == 0) {
      bool success = false;
      auto content = Utility::ReadTextFile(file, &success);
      if (!success) {
        LOG_ERROR("Failed to load scene: {}", file);
        return nullptr;
      }
      std::vector< std::string > separated_jsons;

      std::string delimiter =
          "42091169692137SUPERJSONENTITYSEPARATOR42091169692137";  // TODO: Move to one place
                                                                   // instead of declaring each time
      size_t pos = 0;
      std::string token;
      while ((pos = content.find(delimiter)) != std::string::npos) {
        token = content.substr(0, pos);
        separated_jsons.push_back(token);
        content.erase(0, pos + delimiter.length());
      }

      separated_jsons.push_back(content);

      nlohmann::json sceneJson =
          nlohmann::json::parse(separated_jsons[0].begin(), separated_jsons[0].end());

      size_t sceneID = sceneJson["sceneID"];

      auto current_scene = SceneManager::GetCurrentScene();
      auto scene         = std::make_shared< Scene >(sceneID);
      SceneManager::AddScene(scene);
      SceneManager::OpenScene(scene->GetID());

      auto sg = scene->SceneGraph();

      for (int i = 1; i < separated_jsons.size(); i++) {
        auto entity = ECS::EntityManager::GetInstance().CreateEntity();
        entity->LoadFromJson(separated_jsons[i]);
        // Removed 'Has Editor Camera' check - editor camera should not be serialized
        sg->AddChild(0, entity->GetID());
      }
      if (current_scene != nullptr) {
        SceneManager::OpenScene(current_scene->GetID());
      }
      scene->CameraSystem()->FindMainCamera();
      _loadedScenes[file] = scene;
      return scene;
    }
    return _loadedScenes[file];
  }
}  // namespace Engine
