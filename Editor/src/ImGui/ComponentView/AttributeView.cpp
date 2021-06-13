#include <pch.h>
#include <ImGui/imgui.h>
#include <imgui/imgui_internal.h>
#include "AttributeView.h"

namespace Editor {
  auto DrawVec3(const std::string& name, glm::vec3& value, float resetValue) -> bool {
    float columnWidth = 100.0f;
    ImGuiIO& io       = ImGui::GetIO();
    bool res          = false;

    auto boldFont = io.Fonts->Fonts[0];

    ImGui::PushID(name.c_str());

    ImGui::Columns(2);
    ImGui::SetColumnWidth(0, columnWidth);
    ImGui::Text(name.c_str());
    ImGui::NextColumn();

    ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{2, 2});

    float lineHeight  = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
    ImVec2 buttonSize = {lineHeight + 3.0f, lineHeight};

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.8f, 0.1f, 0.15f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.9f, 0.2f, 0.2f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.8f, 0.1f, 0.15f, 1.0f});
    ImGui::PushFont(boldFont);
    if (ImGui::Button("X", buttonSize)) {
      value.x = resetValue;
      res     = true;
    }
    ImGui::PopFont();
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    res |= ImGui::DragFloat("##X", &value.x, 0.1f, 0.0f, 0.0f, "%.2f");
    ImGui::PopItemWidth();
    ImGui::SameLine();

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.2f, 0.7f, 0.2f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.3f, 0.8f, 0.3f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.2f, 0.7f, 0.2f, 1.0f});
    ImGui::PushFont(boldFont);
    if (ImGui::Button("Y", buttonSize)) {
      value.y = resetValue;
      res     = true;
    }
    ImGui::PopFont();
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    res |= ImGui::DragFloat("##Y", &value.y, 0.1f, 0.0f, 0.0f, "%.2f");
    ImGui::PopItemWidth();
    ImGui::SameLine();

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.1f, 0.25f, 0.8f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.2f, 0.35f, 0.9f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.1f, 0.25f, 0.8f, 1.0f});
    ImGui::PushFont(boldFont);
    if (ImGui::Button("Z", buttonSize)) {
      value.z = resetValue;
      res     = true;
    }
    ImGui::PopFont();
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    res |= ImGui::DragFloat("##Z", &value.z, 0.1f, 0.0f, 0.0f, "%.2f");
    ImGui::PopItemWidth();

    ImGui::PopStyleVar();

    ImGui::Columns(1);

    ImGui::PopID();

    return res;
  }
  auto DrawFloat(const std::string& name, float& value, float min, float max, float sliderSpeed)
      -> bool {
    float columnWidth = 100.0f;
    ImGuiIO& io       = ImGui::GetIO();

    ImGui::PushID(name.c_str());

    ImGui::Columns(2);
    ImGui::SetColumnWidth(0, columnWidth);
    ImGui::Text(name.c_str());
    ImGui::NextColumn();

    ImGui::PushMultiItemsWidths(1, ImGui::CalcItemWidth());
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{2, 2});

    auto res = ImGui::DragFloat("##X", &value, sliderSpeed, min, max, "%.2f");
    ImGui::PopItemWidth();
    ImGui::PopStyleVar();
    ImGui::Columns(1);
    ImGui::PopID();
    return res;
  }
  auto DrawBool(const std::string& name, bool& value) -> void {
    float columnWidth = 100.0f;
    ImGuiIO& io       = ImGui::GetIO();

    ImGui::PushID(name.c_str());
    ImGui::Columns(2);
    ImGui::SetColumnWidth(0, columnWidth);
    ImGui::Text(name.c_str());
    ImGui::NextColumn();

    ImGui::PushMultiItemsWidths(1, ImGui::CalcItemWidth());
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{2, 2});
    ImGui::Checkbox("", &value);

    ImGui::PopItemWidth();
    ImGui::PopStyleVar();
    ImGui::Columns(1);
    ImGui::PopID();
  }
  auto DrawInt(const std::string& name, int& value, int min, int max, int sliderSpeed) -> bool {
    float columnWidth = 100.0f;
    ImGuiIO& io       = ImGui::GetIO();

    ImGui::PushID(name.c_str());

    ImGui::Columns(2);
    ImGui::SetColumnWidth(0, columnWidth);
    ImGui::Text(name.c_str());
    ImGui::NextColumn();

    ImGui::PushMultiItemsWidths(1, ImGui::CalcItemWidth());
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{2, 2});

    auto res = ImGui::DragInt("##X", &value, sliderSpeed, min, max, "%.2f");
    ImGui::PopItemWidth();
    ImGui::PopStyleVar();
    ImGui::Columns(1);
    ImGui::PopID();
    return res;
  }
}  // namespace Editor