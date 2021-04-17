#include "pch.h"
#include "Serializer.h"
#include "Renderer/Material.h"

void Engine::Serializer::SaveMaterial(std::shared_ptr<Renderer::Material> material, std::string filePath)
{
  std::string jsonString = "{";

  jsonString += "\n\tassetID: " + material->GetAssetID();
  jsonString += "\n\tshaderFilepath: " + material->GetShaderFilepath();
  jsonString += "\n\tdiffuseFilepath: " + material->GetDiffuseFilepath();

  jsonString += "\n}";

  std::ofstream ofstream;
  ofstream.open(filePath);

  ofstream << jsonString;
  ofstream.close();

  std::cout << "Material " + std::to_string(material->GetAssetID()) + " saved to file " + filePath;
}
