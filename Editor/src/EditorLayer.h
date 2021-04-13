#pragma once
#include "Engine.h"
#include <string>

using namespace Engine;
template< typename T >
using ptr_t = std::shared_ptr< T >;

class EditorLayer : public Engine::Layer {
public:
  EditorLayer(const std::string& name);
  virtual ~EditorLayer() = default;

  virtual void OnAttach() override;
  virtual void OnUpdate(double deltaTime) override;
  virtual void OnDetach() override;
  virtual void OnEvent(Event& event) override;

private:
  bool OnMouseScroll(MouseScrolledEvent& e);
  bool OnMouseButtonPress(MouseButtonPressedEvent& e);
  bool OnMouseButtonRelease(MouseButtonReleasedEvent& e);

private:
  /*Temporary*/
  float m_Time = 0;

  ptr_t< GL::Shader > m_Shader;
  ptr_t< Renderer::Mesh > m_ConeMesh;
  ptr_t< Renderer::Material > m_Material;

  /*Camera*/
  Camera m_Camera;
  GL::CameraUniformData camera_data;
  GL::CameraUniformBuffer camera_buffer;
  struct {
    glm::vec2 screenSize;
    glm::vec2 m2LastPos;
    glm::vec2 m3LastPos;
    float sensitivity = 1.0f;
    float scrollDelta = 0.0f;
  } mouseState;

  /*ECS*/
  Scene m_Scene;
  ptr_t< ECS::Entity > m_Entity1;
  ptr_t< ECS::Entity > m_Entity2;
};