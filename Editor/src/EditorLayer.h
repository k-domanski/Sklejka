#pragma once
#include "Engine.h"
#include <Components/Transform.h>
#include <string>
#include "EditorCameraArgs.h"
#include "ImGui/Panels/SceneHierarchyPanel.h"
#include "ImGui/Panels/InspectorPanel.h"
#include "ImGui/Panels/FileSystemPanel.h"
#include "ImGui/Panels/MaterialPanel.h"

template< typename T >
using ptr_t = std::shared_ptr< T >;

class EditorLayer : public Engine::Layer {
public:
  EditorLayer(const std::string& name);
  virtual ~EditorLayer() = default;

  virtual void OnAttach() override;
  virtual void OnUpdate(double deltaTime) override;
  virtual void OnDetach() override;
  virtual void OnEvent(Engine::Event& event) override;
  virtual void OnImGuiRender() override;

  auto AddObjectOnScene(const std::string& path, Engine::ECS::EntityID parent = 0) -> void;

private:
  bool OnWindowResize(Engine::WindowResizeEvent& e);
  bool OnMouseScroll(Engine::MouseScrolledEvent& e);
  bool OnMouseButtonPress(Engine::MouseButtonPressedEvent& e);
  bool OnMouseButtonRelease(Engine::MouseButtonReleasedEvent& e);
  auto UpdateEditorCamera() -> void;

private:
  /*Temporary*/
  float m_Time = 0;
  float m_it   = 0;

  ptr_t< Engine::GL::Shader > m_Shader;
  ptr_t< Engine::Renderer::Mesh > m_ConeMesh;
  ptr_t< Engine::Renderer::Material > m_Material;
  std::shared_ptr< Engine::Renderer::Model > m_PepeModel;
  std::shared_ptr< Engine::Renderer::Material > m_PepeMaterial;

  /*Camera*/
  struct {
    std::shared_ptr< Engine::Camera > camera;
    std::shared_ptr< Engine::Transform > transform;
  } m_EditorCamera;
  EditorCameraArgs editorCameraArgs;

  /*ECS*/
  ptr_t< Engine::Scene > m_Scene = nullptr;//scene manager test
  //std::make_shared< Scene >(); 
  ptr_t< Engine::ECS::Entity > m_Entity1;
  ptr_t< Engine::ECS::Entity > m_Entity2;
  std::shared_ptr< Engine::ECS::Entity > m_Pepe;
  std::shared_ptr< Engine::Transform > m_PepeTransform;

  /*Panels*/
  Editor::SceneHierarchyPanel m_SceneHierarchyPanel;
  Editor::InspectorPanel m_InspectorPanel;
  Editor::FileSystemPanel m_FileSystemPanel;
  Editor::MaterialPanel m_MaterialPanel;
};