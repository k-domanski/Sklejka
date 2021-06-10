#include "NodesUtilityPanel.h"
#include <Components/Node.h>
#include <Components/MeshRenderer.h>
#include <regex>

namespace Editor {
  auto NodesUtilityPanel::SetEditorLayer(EditorLayer* layer) -> void {
    _editorLayer = layer;
  }
  auto NodesUtilityPanel::SetScene(const std::shared_ptr< Engine::Scene >& scene) -> void {
    _scene = scene;
  }
  auto NodesUtilityPanel::OnImGuiRender() -> void {
    using Engine::Node;
    using Engine::NodeTag;
    using Engine::Components::MeshRenderer;
    using Engine::ECS::Entity;

    ImGui::Begin("Nodes Utility");
    ImGui::LabelText("Info Text", "Find all nodes on scene and assign them the Node component");
    if (_scene != nullptr) {
      const auto& entities = _scene->Entities();
      const std::regex node_regex("[Nn]ode_[0-9]*_[0-9]*");
      const std::regex boss_regex("[Bb]oss_[0-9]*_[0-9]*");
      const std::regex number_regex("[0-9]+");

      const auto Process = [&entities, &number_regex](const std::regex& type, NodeTag tag) {
        std::vector< std::shared_ptr< Entity > > nodes;
        int count = 0;
        for (auto& entity : entities) {
          if (!std::regex_match(entity->Name(), type)) {
            continue;
          }

          const auto& name = entity->Name();
          std::vector< std::string > numbers(
              std::sregex_token_iterator(name.begin(), name.end(), number_regex),
              std::sregex_token_iterator());
          if (numbers.size() < 2) {
            LOG_WARN("{} has too few indexes", name);
            continue;
          }
          auto self_index = std::stoi(numbers[0]);
          auto next_index = std::stoi(numbers[1]);

          auto node_comp = entity->AddComponent< Node >();
          node_comp->Index(self_index);
          node_comp->NextIndex(next_index);
          node_comp->Tag(tag);
          ++count;

          if (entity->HasComponent< MeshRenderer >()) {
            entity->RemoveComponent< MeshRenderer >();
          }
        }
      };

      if (ImGui::Button("Process")) {
        Process(node_regex, NodeTag::Player);
        Process(boss_regex, NodeTag::Boss);
      }

      /* Display matching entities */
      ImGui::Separator();
      ImGui::BeginChild("Nodes List");
      {
        for (auto& entity : entities) {
          if (!std::regex_match(entity->Name(), node_regex)) {
            continue;
          }
          auto has_component = entity->HasComponent< Node >();
          if (has_component) {
            ImGui::PushStyleColor(ImGuiCol_Text, {0.0f, 0.5f, 0.0f, 1.0f});
          }
          ImGui::TextUnformatted(entity->Name().c_str());
          if (has_component) {
            ImGui::PopStyleColor();
          }
        }
      }
      ImGui::EndChild();
    }
    ImGui::End();
  }
}  // namespace Editor
