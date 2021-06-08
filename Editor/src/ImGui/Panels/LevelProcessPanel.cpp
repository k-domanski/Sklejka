#include "LevelProcessPanel.h"
#include "EditorLayer.h"
#include "MeshMapping.h"
#include <Systems/SceneGraph.h>

using namespace Engine;
using namespace Engine::ECS;

namespace Editor {
  LevelProcessPanel::LevelProcessPanel() {
  }

  auto LevelProcessPanel::SetScene(const std::shared_ptr< Engine::Scene >& scene) -> void {
    _scene = scene;
  }

  auto LevelProcessPanel::SetEditorLayer(EditorLayer* layer) -> void {
    _editorLayer = layer;
  }

  auto LevelProcessPanel::OnImGuiRender() -> void {
    ImGui::Begin("Process Level");
    /* Level 1*/
    if (ImGui::Button("Level 1")) {
      const auto& collider_mapping = GetColliderMapping_1();
      const auto& scene_graph      = _scene->SceneGraph();
      auto& entities               = _scene->RenderSystem()->Entities();
      for (auto& entity : entities) {
        auto renderer = entity->GetComponent< Components::MeshRenderer >();
        auto index    = renderer->MeshIndex();
        auto correct_model =
            renderer->GetModel()->GetFilepath() == "Assets/models/AA_Level1_DZWONY.fbx";
        if (correct_model && ShouldIgnore_1(index)) {
          continue;
        }
        auto srt   = collider_mapping[index];
        auto child = EntityManager::GetInstance().CreateEntity();
        child->Name("Collider");
        scene_graph->SetParent(child, entity);
        child->collisionLayer.ClearAll();

        auto tr = child->AddComponent< Transform >();
        tr->Position(srt.position);
        tr->Euler(glm::radians(srt.rotation));
        tr->Scale(srt.scale);
        auto collider = child->AddComponent< Components::Collider >();
        auto rb       = child->AddComponent< Components::Rigidbody >();
        rb->SetGravity(false);
      }
    }
    ImGui::End();
  }
}  // namespace Editor
