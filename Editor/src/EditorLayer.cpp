#include "EditorLayer.h"

EditorLayer::EditorLayer(const std::string& name): Layer(name) {
}
void EditorLayer::OnAttach() {
  APP_INFO("EditorLayer OnAttach.");
}

void EditorLayer::OnUpdate() {
  APP_INFO("EditorLayer OnUpdate.");
}

void EditorLayer::OnDetach() {
  APP_INFO("EditorLayer OnDetach.");
}
