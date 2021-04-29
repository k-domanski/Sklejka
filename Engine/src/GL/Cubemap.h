#pragma once
#include <pch.h>
#include <GL/Buffer.h>
#include <GL/VertexArray.h>
namespace Engine::GL
{
  class Shader;

  class Cubemap {
  private:
    VertexArray skyboxVAO;
    VertexBuffer skyboxVBO;
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
