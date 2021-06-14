#pragma once
#include <pch.h>
#include <ImGui/imgui.h>

namespace Editor {
  auto DrawVec3(const std::string& name, glm::vec3& value, float resetValue = 0.0f) -> bool;
  auto DrawVec2(const std::string& name, glm::vec2& value, float resetValue = 0.0f) -> bool;
  auto DrawFloat(const std::string& name, float& value, float min = 0.0f, float max = 0.0f,
                 float sliderSpeed = 0.01f) -> bool;
  auto DrawBool(const std::string& name, bool& value) -> void;
  auto DrawInt(const std::string& name, int& value, int min = 0, int max = 0, int sliderSpeed = 1)
      -> bool;
}  // namespace Editor