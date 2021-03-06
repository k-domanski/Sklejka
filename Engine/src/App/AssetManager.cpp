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
  if (fs::exists(path) && !fs::is_regular_file(path)) {
    LOG_WARN("No file in this path: {}", path);
    return false;
  }
  if (!fs::exists(path)) {
    LOG_WARN("File does not exist: {}", path);
    return false;
  }
  return true;
}

auto FolderExists(const std::string& path) -> bool {
  if (fs::exists(path) && !fs::is_directory(path)) {
    LOG_WARN("No folder under this path: {}", path);
    return false;
  }
  if (!fs::exists(path)) {
    LOG_WARN("Folder does not exist: {}", path);
    return false;
  }
  return true;
}

auto PreparePath(const std::string& path) -> std::string {
  const std::string assets = ("Assets");
  auto pos                 = path.find(assets);
  if (pos == std::string::npos)
    return assets + "/" + path;
  return path;
}

#define FORMAT_PATH(path)                                                                          \
  {                                                                                                \
    path = StripToRelativePath(path);                                                              \
    path = PreparePath(path);                                                                      \
    if (FileExists(path) || FolderExists(path)) {                                                  \
      path = fs::relative(fs::canonical(path)).generic_string();                                   \
    }                                                                                              \
  }

namespace Engine {
  FolderPaths AssetManager::_folderPahts{
      "./Assets/",            //
      "./Assets/fonts/",      //
      "./Assets/materials/",  //
      "./Assets/models/",     //
      "./Assets/resources/",  //
      "./Assets/scenes/",     //
      "./Assets/shaders/",    //
      "./Assets/skyboxes/",   //
      "./Assets/sounds/",     //
      "./Assets/textures/",   //
  };

  auto AssetManager::GetAssetsFolders() -> FolderPaths {
    return _folderPahts;
  }
  auto AssetManager::GenerateAssetID() -> size_t {
    std::random_device dv;
    std::mt19937_64 mt(dv());
    return static_cast< size_t >(mt());
  }

  auto AssetManager::GetWoringDir() -> std::string {
    return std::filesystem::current_path().string();
  }

  auto AssetManager::GetShader(std::string file) -> std::shared_ptr< GL::Shader > {
    FORMAT_PATH(file);
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
      GL::Context::BindUniformBlocks(shader);
      return shader;
    }
    return _loadedShaders[file];
  }
  auto AssetManager::GetModel(std::string file) -> std::shared_ptr< Renderer::Model > {
    FORMAT_PATH(file);
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
    FORMAT_PATH(file);
    if (_loadedTextures2D.count(file) == 0) {
      if (!FileExists(file)) {
        return nullptr;
      }
      int x, y, n;
      // auto pixel_data = stbi_loadf(file.data(), &x, &y, &n, 4);
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
  auto AssetManager::CreateMaterial(std::string filePath) -> std::shared_ptr< Renderer::Material > {
    FORMAT_PATH(filePath);
    auto assetID              = GenerateAssetID();
    auto material             = std::make_shared< Renderer::Material >(assetID);
    _loadedMaterials[assetID] = material;
    material->FilePath(filePath);
    return material;
  }
  auto AssetManager::GetMaterial(std::string file) -> std::shared_ptr< Renderer::Material > {
    /* Check if material from this file is already loaded */
    FORMAT_PATH(file);
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
      if (json.count("tilingOffset") > 0) {
        auto vec = json["tilingOffset"].get< std::vector< float > >();
        material->TilingOffset({vec[0], vec[1], vec[2], vec[3]});
      } else {
        material->TilingOffset(glm::vec4(1.0f, 1.0f, 0.0f, 0.0f));
      }
      material->Roughness(READ_VALUE(json, "roughness", 0));
      material->Metalness(READ_VALUE(json, "metalness", 0));
      material->RoughnessMap(GetTexture2D(READ_VALUE(json, "roughnessMap", "")));
      material->MetalnessMap(GetTexture2D(READ_VALUE(json, "metalnessMap", "")));

      /* Stencli */
      using namespace std::literals;
      material->UseStencil(READ_VALUE(json, "useStencil", false));
      material->StencilRef(READ_VALUE(json, "stencilRef", 0));
      material->StencilFunc(
          GL::StencilFunc::_from_string(READ_VALUE(json, "stencilFunc", "Always"s).c_str()));
      material->StencilSFail(
          GL::StencilOp::_from_string(READ_VALUE(json, "sfail", "Keep"s).c_str()));
      material->StencilZFail(
          GL::StencilOp::_from_string(READ_VALUE(json, "zfail", "Keep"s).c_str()));
      material->StencilZPass(
          GL::StencilOp::_from_string(READ_VALUE(json, "zpass", "Keep"s).c_str()));

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
    auto entities               = sg->GetChildren(nullptr);

    using namespace nlohmann;
    json json = nlohmann::json{
        {"sceneID", SceneManager::GetCurrentScene()->GetID()},
    };

    std::string fileContent = json.dump(4);

    for (int i = 0; i < entities.size(); i++) {
      auto entity   = entities[i];
      auto children = sg->GetChildren(entity);

      for (auto child : children) {
        entities.push_back(child);  // Add each child to list of entities to serialize
      }

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
      auto parent      = sg->GetParent(entity);
      auto parent_id   = parent != nullptr ? parent->GetID() : 0;
      auto entity_json = entity->SaveToJson(parent_id);

      fileContent.append(entity_json);
    }

    std::ofstream ofstream;
    ofstream.open(file);
    ofstream << fileContent;
    ofstream.close();
  }
  auto AssetManager::LoadScene(std::string file) -> std::shared_ptr< Scene > {
    FORMAT_PATH(file);
    // if (_loadedScenes.count(file) == 0) {
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
      // TODO: May fail if parent is not deserialized yet
      sg->AddChild(ECS::EntityManager::GetInstance().GetEntity(parentID), entity);
    }

    scene->CameraSystem()->FindMainCamera();
    _loadedScenes[file] = scene;

    if (current_scene != nullptr) {
      SceneManager::OpenScene(current_scene->GetID());
    }
    // return std::make_shared<Scene>(*scene);
    return scene;
    //}
    //    return std::make_shared< Scene > (*_loadedScenes[file]);
    // return _loadedScenes[file];
  }

  auto AssetManager::GetCharacters(std::string file, int fontSize)
      -> std::shared_ptr< std::map< char, Utility::Character > > {
    FORMAT_PATH(file);
    auto characters = std::make_shared< std::map< char, Utility::Character > >();

    if (_loadedCharacters.count(file) == 0) {
      if (!FileExists(file)) {
        return nullptr;
      }
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

      glPixelStorei(GL_UNPACK_ALIGNMENT, 1);  // disable byte-alignment restriction

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
        // glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width,
        // face->glyph->bitmap.rows, 0,
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
  BETTER_ENUM(__FACES, int, RIGHT, LEFT, TOP, BOTTOM, FRONT, BACK);
  auto AssetManager::GetCubemap(std::string folder) -> std::shared_ptr< GL::Cubemap > {
    FORMAT_PATH(folder);
    if (_loadedCubemaps.count(folder) == 0) {
      const std::vector< std::string > extensions{".png", ".jpg"};
      std::vector< std::pair< const unsigned char*, glm::ivec2 > > faces(6,
                                                                         {nullptr, glm::ivec2(0)});
      /*
        Face order:
        Right
        Left
        Top
        Bottom
        Front
        Back
      */
      /* Get all files in this folder */
      std::vector< fs::path > files;
      for (const auto& entry : fs::directory_iterator(folder)) {
        /* If is not file or is not .png nor .jpg */
        if (!entry.is_regular_file()) {
          continue;
        }
        if (!std::any_of(
                extensions.begin(), extensions.end(),
                [ext = entry.path().extension()](const auto& item) { return ext == item; })) {
          continue;
        }
        /* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
        files.push_back(entry.path());
      }
      if (files.size() < 6) {
        LOG_ERROR("Too few files in directory {} to load cubemap", folder);
        return nullptr;
      }
      for (const auto& path : files) {
        auto file_name = path.filename().stem().string();
        std::transform(file_name.begin(), file_name.end(), file_name.begin(), ::toupper);
        auto face = __FACES::_from_string(file_name.c_str());
        int width{0}, height{0}, channels{0};
        std::pair< const unsigned char*, glm::ivec2 > data;
        data.first              = stbi_load(path.string().c_str(), &width, &height, &channels, 4);
        data.second             = {width, height};
        faces[face._to_index()] = data;
      }

      auto cubemap            = std::make_shared< GL::Cubemap >(faces);
      _loadedCubemaps[folder] = cubemap;

      /* Cleanup */
      for (auto pair : faces) {
        if (pair.first != nullptr) {
          stbi_image_free((void*)pair.first);
        }
      }
      /* -=-=-=- */

      return cubemap;
    }
    return _loadedCubemaps[folder];
  }
}  // namespace Engine
