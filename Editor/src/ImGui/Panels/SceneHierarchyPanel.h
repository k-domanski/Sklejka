#pragma once
#include "Engine.h"

namespace Editor {
  class SceneHierarchyPanel {
  public:
    SceneHierarchyPanel();
    SceneHierarchyPanel(std::shared_ptr< Engine::Scene > scene);

    std::shared_ptr< Engine::ECS::Entity > GetSelectedEntity() {
      return m_SelectedEntity;
    }
    void SetSelectedEntity(const std::shared_ptr< Engine::ECS::Entity >& entity) {
      m_SelectedEntity = entity;
      _selectedCallback(entity);
    }
    void SetSelectionCallback(
        const std::function< void(const std::shared_ptr< Engine::ECS::Entity >) >& callback) {
      _selectedCallback = callback;
    }
    void SetScene(std::shared_ptr< Engine::Scene > scene);
    void OnImGuiRender();

  private:
    void DrawEntity(std::shared_ptr< Engine::ECS::Entity > entity);

  private:
    std::shared_ptr< Engine::Scene > m_Scene;
    std::shared_ptr< Engine::ECS::Entity > m_SelectedEntity;
    std::function< void(const std::shared_ptr< Engine::ECS::Entity >) > _selectedCallback;
  };
}  // namespace Editor