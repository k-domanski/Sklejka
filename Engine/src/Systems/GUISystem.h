#pragma once
#include "ECS/System.h"
#include "GL/Shader.h"
#include "Renderer/Mesh.h"
#include "Renderer/PingPongBuffer.h"

namespace Engine::Systems {
  class GUISystem : public Engine::ECS::System {
  public:
    GUISystem();
    void Update(float deltaTime) override;
    auto OnWindowResize(glm::vec2 windowSize) -> void;
    auto HandleMousePressed(glm::vec2 mousePosition) -> void;
    auto HandleMouseRelease(glm::vec2 mousePosition) -> void;
    auto BreakChecking(bool breakChecking) -> void;

  private:
    std::shared_ptr< Engine::GL::Shader > _textShader;
    std::shared_ptr< Engine::GL::Shader > _imageShader;
    glm::mat4 _camProj;
    bool _breakChecking;
    bool _useMouse;
    bool _DPADUpLastFrame;
    bool _DPADDownLastFrame;
    bool _gamepadALastFrame;
  };
}  // namespace Engine::Systems
