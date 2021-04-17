#pragma once
#include "Systems/Renderer.h"

namespace Engine {
  class Serializer {
  public:
    Serializer()          = default;
    virtual ~Serializer() = default;
    void SaveMaterial(std::shared_ptr< Renderer::Material > material, std::string filePath);
    auto LoadMaterial(std::string filePath);
  };
}  
