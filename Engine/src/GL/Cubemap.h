#pragma once
#include <pch.h>
namespace Engine::GL
{
  class Shader;

  class Cubemap {
  private:
    unsigned int skyboxVAO;
    unsigned int cubemapTexture;
    std::shared_ptr< Shader > skyboxShader;
    std::vector< std::string > faces;
    void Create(std::string path);
    unsigned int LoadCubemap(std::vector< std::string > faces);

  public:
    Cubemap(std::string path);
    void Draw(glm::mat4 view, glm::mat4 projection);
  };

}
