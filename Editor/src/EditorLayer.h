#pragma once
#include "Engine.h"
#include <string>

using namespace Engine;
class EditorLayer : public Engine::Layer {
public:
  EditorLayer(const std::string& name);
  virtual ~EditorLayer() = default;

  virtual void OnAttach() override;
  virtual void OnUpdate(double deltaTime) override;
  virtual void OnDetach() override;
  
private:
	//Temporary
	float m_Time = 0;
	std::shared_ptr< GL::Shader> m_Shader;
	Renderer::Mesh* m_ConeMesh;
	GL::CameraUniformData camera_data;
	GL::CameraUniformBuffer camera_buffer;
};