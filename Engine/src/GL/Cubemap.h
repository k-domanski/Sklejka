#pragma once
#include <pch.h>
#include <GL/Buffer.h>
#include <GL/VertexArray.h>
namespace Engine::GL {
  class Shader;

  class Cubemap {
  private:
    unsigned int _handle;
    std::vector< std::string > faces;
    void Create(std::vector< std::pair< const unsigned char*, glm::ivec2 > > faces);

  public:
    Cubemap(std::vector< std::pair< const unsigned char*, glm::ivec2 > > faces);
    void Draw(glm::mat4 view, glm::mat4 projection);
    auto Bind(GLuint slot) -> void;
  };

}  // namespace Engine::GL
