#pragma once
#include "Engine.h"

using namespace Engine;

namespace EditorGUI {
  class InspectorPanel {
  public:
    InspectorPanel() = default;

    void OnImGuiRender(std::shared_ptr< ECS::Entity > entity);

  private:
    void DrawComponents(std::shared_ptr< ECS::Entity > entity);
  };
}  // namespace EditorGUI