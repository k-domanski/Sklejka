#pragma once
#include <pch.h>
#include <GL/GLCore.h>

namespace Engine::Renderer {
  class Material {
  private:
    std::size_t _assetID;
    GLuint _shaderHandle;

  public:
    Material(std::size_t assetID);
  };
}