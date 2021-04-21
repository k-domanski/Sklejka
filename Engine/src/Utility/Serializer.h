#pragma once
#include "Systems/Renderer.h"

namespace Engine::Serializer {
  bool SaveMaterial(std::shared_ptr< Renderer::Material > material);
  bool SaveMaterial(std::shared_ptr< Renderer::Material > material, std::string filePath);
}  // namespace Engine::Serializer
