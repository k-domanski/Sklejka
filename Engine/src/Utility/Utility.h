#pragma once
#include <pch.h>

namespace Engine::Utility {
  struct ShaderParseResult {
    bool success = false;
    std::string version;
    std::string vertexShader;
    std::string fragmentShader;
    std::string infoMessage;
  };

  auto ReadTextFile(const std::string_view& fileName) -> std::string;
  auto ParseShaderSource(std::string source) -> ShaderParseResult;
}