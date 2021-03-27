#include "pch.h"
#include "Utility.h"

auto Engine::Utility::ReadTextFile(const std::string_view& fileName) -> std::string {
  std::ifstream file(fileName.data());
  if (!file.good()) {
    return "";
  }

  std::string content((std::istreambuf_iterator< char >(file)), std::istreambuf_iterator< char >());
  return content;
}
