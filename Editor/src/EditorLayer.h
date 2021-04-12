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
	//Temporary
	float m_Time = 0;
	ptr_t< GL::Shader> m_Shader;
	Renderer::Mesh* m_ConeMesh;
	//Camera
	Camera m_Camera;
	GL::CameraUniformData camera_data;
	GL::CameraUniformBuffer camera_buffer;
	struct {
		bool m2Pressed = false;
		bool m3Pressed = false;
		bool m2FirstPress = true;
		bool m3FirstPress = true;
		glm::vec2 screenSize;
		glm::vec2 m2LastPos;
		glm::vec2 m3LastPos;
		float sensitivity = 1.0f;
		float scrollDelta = 0.0f;
	} mouseState;
	//ECS + material
	ptr_t<ECS::Entity> m_Entity;
	ptr_t<Renderer::Material> m_Material;
};