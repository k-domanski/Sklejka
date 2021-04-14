#pragma once
#include <pch.h>

struct EditorCameraArgs {
  glm::vec2 screenSize;
  glm::vec2 m2LastPos;
  glm::vec2 m3LastPos;
  float sensitivity = 1.0f;
  float scrollDelta = 0.0f;
};