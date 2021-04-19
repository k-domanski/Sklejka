#include "pch.h"
#include "Serializer.h"
#include "Renderer/Material.h"

#include <regex>

#include "App/AssetManager.h"

void Engine::Serializer::SaveMaterial(std::shared_ptr< Renderer::Material > material,
                                      std::string filePath) {

  std::ofstream ofstream;
  ofstream.open(filePath);

  ofstream << material->ToJson();
  ofstream.close();

  std::cout << "Material " + std::to_string(material->GetAssetID()) + " saved to file " + filePath;
}

std::shared_ptr< Engine::Renderer::Material > Engine::Serializer::LoadMaterial(std::string filePath) {
  std::ifstream ifstream(filePath);
  nlohmann::json json;
  ifstream >> json;

  std::string assetID_string = json["assetID"];
  std::stringstream ss(assetID_string);
  size_t material_assetID;
  ss >> material_assetID;

  std::shared_ptr< GL::Shader > shader_ptr = AssetManager::GetShader(json["shaderFilepath"]);
  std::shared_ptr< GL::Texture2D > texture_ptr = AssetManager::GetTexture2D(json["diffuseFilepath"]);

  return AssetManager::GetMaterial(shader_ptr, texture_ptr, material_assetID);
}
