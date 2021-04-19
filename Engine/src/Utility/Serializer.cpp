#include "pch.h"
#include "Serializer.h"
#include "Renderer/Material.h"

#include <regex>

#include "App/AssetManager.h"

void Engine::Serializer::SaveMaterial(std::shared_ptr< Renderer::Material > material,
                                      std::string filePath) {
  std::string jsonString = "{";

  jsonString += "\n\t\"assetID\" : \"" + std::to_string(material->GetAssetID()) + "\",";
  jsonString += "\n\t\"shaderFilepath\" : \"" + material->GetShaderFilepath() + "\",";
  jsonString += "\n\t\"diffuseFilepath\" : \"" + material->GetDiffuseFilepath() + "\"";

  jsonString += "\n}";

  std::ofstream ofstream;
  ofstream.open(filePath);

  ofstream << jsonString;
  ofstream.close();

  std::cout << "Material " + std::to_string(material->GetAssetID()) + " saved to file " + filePath;
}

std::shared_ptr< Engine::Renderer::Material > Engine::Serializer::LoadMaterial(std::string filePath) {
  std::ifstream ifstream;
  ifstream.open(filePath);

  std::vector< std::string > jsonLines;
  char line[1024];

  while (!ifstream.eof()) {
    ifstream.getline(line, 1024);
    jsonLines.push_back(line);
  }
  ifstream.close();

  std::string assetID_string = "";
  std::string shaderFilepath_string = "";
  std::string diffuseFilepath_string = "";

  std::regex json_regex("(.*)(\".*\") [:] (\".*\")(.*)");

  std::regex assetID_regex("(.*)(\"assetID\") [:] (\".*\")(.*)");
  std::regex shader_filepath_regex("(.*)(\"shaderFilepath\") [:] (\".*\")(.*)");
  std::regex diffuse_filepath_regex("(.*)(\"diffuseFilepath\") [:] (\".*\")(.*)");

  for (auto json_line : jsonLines) {
    if (std::regex_match(json_line, json_regex)) {
     
      if (std::regex_match(json_line, assetID_regex)) {
        std::size_t colon_pos           = json_line.find(":");
        std::size_t last_quote_mark_pos = json_line.find_last_of("\"");
        assetID_string =
            json_line.substr(colon_pos + 3, last_quote_mark_pos - colon_pos - 3);

      }

      if (std::regex_match(json_line, shader_filepath_regex)) {
        std::size_t colon_pos           = json_line.find(":");
        std::size_t last_quote_mark_pos = json_line.find_last_of("\"");
        shaderFilepath_string =
            json_line.substr(colon_pos + 3, last_quote_mark_pos - colon_pos - 3);

      }

      if (std::regex_match(json_line, diffuse_filepath_regex)) {
        std::size_t colon_pos           = json_line.find(":");
        std::size_t last_quote_mark_pos = json_line.find_last_of("\"");
        diffuseFilepath_string =
            json_line.substr(colon_pos + 3, last_quote_mark_pos - colon_pos - 3);

      }
    }
  }
  std::stringstream ss(assetID_string);
  size_t material_assetID;
  ss >> material_assetID;

  std::shared_ptr< GL::Shader > shader_ptr = AssetManager::GetShader(shaderFilepath_string);
  std::shared_ptr< GL::Texture2D > texture_ptr = AssetManager::GetTexture2D(diffuseFilepath_string);

  return AssetManager::GetMaterial(shader_ptr, diffuseFilepath_string,
                                   texture_ptr);
  ;
}
