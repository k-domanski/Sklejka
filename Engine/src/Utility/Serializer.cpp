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
