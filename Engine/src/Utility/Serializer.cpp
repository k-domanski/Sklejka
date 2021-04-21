#include "pch.h"
#include "Serializer.h"
#include "Renderer/Material.h"

#include <regex>

#include "App/AssetManager.h"

namespace Engine::Serializer {
  bool SaveMaterial(std::shared_ptr< Renderer::Material > material) {
    if (material == nullptr) {
      LOG_ERROR("No material provided for serialization");
      return false;
    }
    return SaveMaterial(material, material->FilePath());
  }
  bool SaveMaterial(std::shared_ptr< Renderer::Material > material, std::string filePath) {
    if (material == nullptr) {
      LOG_ERROR("No material provided for serialization");
      return false;
    }
    std::ofstream file_stream;
    file_stream.open(filePath);
    if (!file_stream.good()) {
      LOG_ERROR("Failed to create file {}", filePath);
      return false;
    }

    file_stream << material->ToJson();
    file_stream.close();

    std::cout << "Material " + std::to_string(material->GetAssetID()) + " saved to file "
                     + filePath;
    return true;
  }
}  // namespace Engine::Serializer