#include "pch.h"
#include "Cubemap.h"
#include "Renderer/Vertex.h"

#include "App/AssetManager.h"
#include "GL/Context.h"

Engine::GL::Cubemap::Cubemap(std::vector< std::pair< const unsigned char*, glm::ivec2 > > faces) {
  Create(faces);
}

void Engine::GL::Cubemap::Create(
    std::vector< std::pair< const unsigned char*, glm::ivec2 > > faces) {
  /* Preserve state */
  auto current_target  = Context::GetTextureTarget();
  auto current_texture = Context::GetBoundTexture();

  glGenTextures(1, &_handle);
  glBindTexture(GL_TEXTURE_CUBE_MAP, _handle);
  for (int index = 0; index < faces.size(); ++index) {
    const auto& data = faces[index];
    if (data.first == nullptr) {
      LOG_WARN("Nullptr in data while creating cubemap");
      continue;
    }
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + index, 0, GL_RGBA, data.second.x, data.second.y,
                 0, GL_RGBA, GL_UNSIGNED_BYTE, data.first);
  }
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

  /* Restore state */
  Context::BindTexture(current_target, current_texture);
}

/* Don't use that method */
void Engine::GL::Cubemap::Draw(glm::mat4 view, glm::mat4 projection) {
  view = glm::mat4(glm::mat3(view));  // remove translation from the view matrix

  glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to
                           // depth buffer's content
  // skyboxShader->Use();
  // skyboxShader->SetMatrix("view", view);
  // skyboxShader->SetMatrix("projection", projection);
  //// skybox cube
  // skyboxVAO.Bind();
  // skyboxVBO.Bind();
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, _handle);
  glDrawArrays(GL_TRIANGLES, 0, 36);
  glDepthFunc(GL_LESS);  // set depth function back to default
}

auto Engine::GL::Cubemap::Bind(GLuint slot) -> void {
  Context::BindTexture(GL_TEXTURE_CUBE_MAP, _handle, slot);
}
