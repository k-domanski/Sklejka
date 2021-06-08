#pragma once
#include "Engine.h"
#include <Components/Transform.h>
#include <string>
#include "EditorCameraArgs.h"
#include "ImGui/Panels/SceneHierarchyPanel.h"
#include "ImGui/Panels/InspectorPanel.h"
#include "ImGui/Panels/FileSystemPanel.h"
#include "ImGui/Panels/MaterialPanel.h"
#include "ImGui/Panels/NodesUtilityPanel.h"
#include "ImGui/Panels/LevelProcessPanel.h"

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

  auto AddObjectOnScene(const std::string& path, std::shared_ptr< ECS::Entity > parent = nullptr) -> void;
 /* auto AddObjectOnScene(std::shared_ptr< Renderer::Model > model, int meshIndex,
                        Engine::ECS::EntityID parent, std::vector< ECS::EntityID >* loadedMeshes)
      -> ECS::EntityID;*/

private:
  bool OnWindowResize(Engine::WindowResizeEvent& e);
  bool OnMouseScroll(Engine::MouseScrolledEvent& e);
  bool OnMouseButtonPress(Engine::MouseButtonPressedEvent& e);
  bool OnMouseButtonRelease(Engine::MouseButtonReleasedEvent& e);
  bool OnKeyPress(Engine::KeyPressedEvent& e);
  auto UpdateEditorCamera() -> void;
  auto OpenModel() -> void;
  auto DrawMenuBar() -> void;
  auto SaveScene() -> void;
  auto LoadScene() -> void;

private:
  /*Camera*/
  struct {
    std::shared_ptr< Engine::ECS::Entity > entity;
    std::shared_ptr< Engine::Camera > camera;
    std::shared_ptr< Engine::Transform > transform;
  } m_EditorCamera;
  EditorCameraArgs editorCameraArgs;

  /*Scene*/
  ptr_t< Engine::Scene > m_Scene;

  /*Panels*/
  Editor::SceneHierarchyPanel m_SceneHierarchyPanel;
  Editor::InspectorPanel m_InspectorPanel;
  Editor::FileSystemPanel m_FileSystemPanel;
  Editor::MaterialPanel m_MaterialPanel;
  Editor::NodesUtilityPanel m_NodeUtilityPanel;
  Editor::LevelProcessPanel m_LevelProcessPanel;
};