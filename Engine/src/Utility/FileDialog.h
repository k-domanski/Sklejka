#pragma once
#include <optional>

namespace Engine {
  namespace FileDialog {

    std::optional< std::string > OpenFile(const char* filter);
    std::optional< std::string > SaveFile(const char* filter);

  }  // namespace FileDialog
}  // namespace Engine
