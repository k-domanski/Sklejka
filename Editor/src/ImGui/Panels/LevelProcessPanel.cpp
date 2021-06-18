#include "LevelProcessPanel.h"
#include "EditorLayer.h"
#include "MeshMapping.h"
#include <Systems/SceneGraph.h>
#include <regex>

using namespace Engine;
using namespace Engine::ECS;
using namespace Engine::Components;

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
    if (ImGui::Button("Bells")) {
      const std::regex bell_rx("BELL.*");
      for (auto& entity : SceneManager::GetCurrentScene()->Entities()) {
        if (!std::regex_match(entity->Name(), bell_rx)) {
          continue;
        }
        ProcessBell(entity);
        LOG_INFO("Processed {}", entity->Name());
      }
    }
    /* Level 1*/
    if (ImGui::Button("Colliders Level 1")) {
      const auto& collider_mapping = GetColliderMapping_1();
      auto& entities               = _scene->RenderSystem()->Entities();
      AddColliders(collider_mapping, entities, ShouldIgnore_1);
    }
    if (ImGui::Button("Materials Level 1")) {
      auto& entities = _scene->RenderSystem()->Entities();
      for (auto& entity : entities) {
        auto renderer = entity->GetComponent< MeshRenderer >();
        auto mt       = GetMeshMaterial_1(renderer->MeshIndex());
        renderer->SetMaterial(mt);
      }
    }
    /* -=-=-=-=- */

    /* Level 2*/
    if (ImGui::Button("Colliders Level 2")) {
      const auto& collider_mapping = GetColliderMapping_2();
      const auto& tree_mapping     = TreeCollider_2();
      auto& entities               = _scene->RenderSystem()->Entities();
      auto IgnoreFunc = [](int index) -> bool { return ShouldIgnore_2(index) || IsTree_2(index); };
      AddColliders(collider_mapping, entities, IgnoreFunc);
      AddTrees(tree_mapping, entities, IsTree_2);
    }
    if (ImGui::Button("Materials Level 2")) {
      auto& entities = _scene->RenderSystem()->Entities();
      for (auto& entity : entities) {
        auto renderer = entity->GetComponent< MeshRenderer >();
        auto mt       = GetMeshMaterial_2(renderer->MeshIndex());
        renderer->SetMaterial(mt);
      }
    }
    /* -=-=-=-=- */
    ImGui::End();
  }
  auto LevelProcessPanel::AddColliders(const std::vector< Editor::TransformData >& collider_mapping,
                                       std::vector< std::shared_ptr< Entity > > entities,
                                       std::function< bool(int) > IgnoreOp) -> void {
    const auto& scene_graph = _scene->SceneGraph();
    for (auto& entity : entities) {
      auto renderer = entity->GetComponent< Components::MeshRenderer >();
      auto index    = renderer->MeshIndex();

      auto& children = scene_graph->GetChildren(entity);
      auto it        = std::find_if(children.begin(), children.end(),
                             [](auto& item) { return item->Name() == "Collider"; });
      if (it != children.end()) {
        EntityManager::GetInstance().RemoveEntity(*it);
      }

      if (IgnoreOp(index)) {
        continue;
      }

      auto srt   = collider_mapping[index];
      auto child = EntityManager::GetInstance().CreateEntity();
      child->Name("Collider");
      scene_graph->SetParent(child, entity);
      child->collisionLayer.ClearAll();
      child->collisionLayer.Set(LayerMask::Flag::Player);
      child->layer.Set(LayerMask::Flag::Default);

      auto tr = child->AddComponent< Transform >();
      tr->Position(srt.position);
      tr->Euler(glm::radians(srt.rotation));
      tr->Scale(srt.scale);
      auto collider       = child->AddComponent< Components::Collider >();
      collider->IsTrigger = true;
      auto rb             = child->AddComponent< Components::Rigidbody >();
      rb->SetGravity(false);
    }
  }
  auto LevelProcessPanel::AddTrees(const std::vector< Editor::TreeData >& collider_mapping,
                                   std::vector< std::shared_ptr< Entity > > entities,
                                   std::function< bool(int) > IsTreeOp) -> void {
    const auto& scene_graph = _scene->SceneGraph();
    for (auto& entity : entities) {
      auto renderer = entity->GetComponent< Components::MeshRenderer >();
      auto index    = renderer->MeshIndex();

      auto& children = scene_graph->GetChildren(entity);
      auto it        = std::find_if(children.begin(), children.end(),
                             [](auto& item) { return item->Name() == "Collider Tree"; });
      if (it != children.end()) {
        EntityManager::GetInstance().RemoveEntity(*it);
      }

      if (!IsTreeOp(index)) {
        continue;
      }

      auto tree    = collider_mapping[TreeColliderIndex_2(index)];
      auto child_c = EntityManager::GetInstance().CreateEntity();
      auto child_t = EntityManager::GetInstance().CreateEntity();
      child_c->Name("Collider Tree");
      child_t->Name("Collider Tree");

      scene_graph->SetParent(child_c, entity);
      scene_graph->SetParent(child_t, entity);

      child_c->collisionLayer.ClearAll();
      child_c->collisionLayer.Set(LayerMask::Flag::Player);
      child_c->layer.Set(LayerMask::Flag::Default);

      child_t->collisionLayer.ClearAll();
      child_t->collisionLayer.Set(LayerMask::Flag::Player);
      child_t->layer.Set(LayerMask::Flag::Default);

      auto tr_c = child_c->AddComponent< Transform >();
      tr_c->Position(tree.crown.position);
      tr_c->Euler(glm::radians(tree.crown.rotation));
      tr_c->Scale(tree.crown.scale);

      auto tr_t = child_t->AddComponent< Transform >();
      tr_t->Position(tree.trunk.position);
      tr_t->Euler(glm::radians(tree.trunk.rotation));
      tr_t->Scale(tree.trunk.scale);

      auto collider_c       = child_c->AddComponent< Components::Collider >();
      collider_c->IsTrigger = true;

      auto collider_t       = child_t->AddComponent< Components::Collider >();
      collider_t->IsTrigger = true;

      auto rb_c = child_c->AddComponent< Components::Rigidbody >();
      rb_c->SetGravity(false);

      auto rb_t = child_t->AddComponent< Components::Rigidbody >();
      rb_t->SetGravity(false);
    }
  }
  auto LevelProcessPanel::ProcessBell(std::shared_ptr< Entity >& entity) -> void {
    entity->layer.Set(LayerMask::Flag::Default | LayerMask::Flag::Acorn);
    entity->collisionLayer.Set(LayerMask::Flag::Player);
    auto collider       = entity->AddComponent< Collider >();
    collider->Center    = {0.0f, 0.0f, 5.0f};
    collider->Size      = {1.4f, -6.8f, 6.7f};
    collider->Static    = true;
    collider->IsTrigger = true;

    auto rb = entity->AddComponent< Rigidbody >();
    rb->SetKinematic(true);
    rb->SetGravity(false);

    auto mr = entity->GetComponent< MeshRenderer >();
    mr->SetMaterial(AssetManager::GetMaterial("Assets/materials/Bell.mat"));
  }
}  // namespace Editor
