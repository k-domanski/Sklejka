#pragma once
#include "ECS/System.h"
#include "GL/Shader.h"

namespace Engine::Systems
{
  class GUISystem : public Engine::ECS::System {
  public:
    GUISystem();
    void Update(float deltaTime) override;
    auto OnWindowResize(glm::vec2 windowSize) -> void;
    auto HandleMousePressed(glm::vec2 mousePosition) -> void;
    auto HandleMouseRelease(glm::vec2 mousePosition) -> void;

  private:
    std::shared_ptr< Engine::GL::Shader > _textShader;
    std::shared_ptr< Engine::GL::Shader > _imageShader;
    glm::mat4 _camProj;
  };
}
