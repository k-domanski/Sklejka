#pragma once
#include "Engine.h"
#include <ImGui/ComponentView/ComponentView.h>

using namespace Engine;

namespace Editor {
  class InspectorPanel {
  private:
    std::shared_ptr< ECS::Entity > _entity;
    std::vector< std::shared_ptr< IComponentView > > _componentViews;

  public:
    InspectorPanel() = default;

    void OnImGuiRender();
    auto AttachEntity(const std::shared_ptr< ECS::Entity >& entity) -> void;
    auto ReattachEntity() -> void;

  private:
    auto PopulateComponentViews() -> void;
    auto WrapViewInTreeNode(const std::shared_ptr< IComponentView >& view) -> void;
    auto ShowComponentsDropdown() -> void;
  };
}  // namespace Editor