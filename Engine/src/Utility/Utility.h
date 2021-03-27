#pragma once
#include <pch.h>

namespace Engine::Utility {
  auto ReadTextFile(const std::string_view& fileName) -> std::string;
}