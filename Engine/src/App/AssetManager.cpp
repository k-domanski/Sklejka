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

#include "freetype/freetype.h"

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

    for (int i = 0; i < entities_ids.size(); i++) {
      auto id = entities_ids[i];
      std::cout << "\nSaving entity " << id;
      auto children = sg->GetChildren(id);

      for (auto child : children) {
        entities_ids.push_back(child);  // Add each child to list of entities to serialize
        std::cout << "\nadding child " << child << " to entities list";
      }

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

      auto entity_json = entity->SaveToJson(sg->GetParent(entity->GetID()));

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
        auto parentID = entity->GetParentFromJson(separated_jsons[i]);
        // Removed 'Has Editor Camera' check - editor camera should not be serialized
        sg->AddChild(parentID, entity->GetID());
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

  auto AssetManager::GetCharacters(std::string file, int fontSize)
      -> std::shared_ptr<std::map< char, Utility::Character >> {
    auto characters = std::make_shared< std::map< char, Utility::Character > >();

    if (_loadedCharacters.count(file) == 0)
    {
      FT_Library ft;
      if (FT_Init_FreeType(&ft)) {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        return characters;
      }

      FT_Face face;
      if (FT_New_Face(ft, file.c_str(), 0, &face)) {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
        return characters;
      }

      FT_Set_Pixel_Sizes(face, 0, fontSize);

      if (FT_Load_Char(face, 'X', FT_LOAD_RENDER)) {
        std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
        return characters;
      }

      glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction

      for (unsigned char c = 0; c < 128; c++) {
        // load character glyph
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
          std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
          continue;
        }
        // generate texture

        std::shared_ptr< GL::Texture2D > texture = std::make_shared< GL::Texture2D >(
            face->glyph->bitmap.width, face->glyph->bitmap.rows, face->glyph->bitmap.buffer,
            GL_CLAMP_TO_EDGE, GL_LINEAR, GL_RED, GL_UNSIGNED_BYTE);

        // unsigned int texture;
        // glGenTextures(1, &texture);
        // glBindTexture(GL_TEXTURE_2D, texture);
        // glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows,
        // 0,
        //             GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
        //// set texture options
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // now store character for later use
        Character character = {
            texture, glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top), face->glyph->advance.x};
        characters->insert(std::pair< char, Character >(c, character));
      }

      _loadedCharacters[file] = characters;

      FT_Done_Face(face);
      FT_Done_FreeType(ft);

      return characters;
    }

    return _loadedCharacters[file];
  }
}  // namespace Engine
