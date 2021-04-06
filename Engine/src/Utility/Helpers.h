#pragma once
#include <pch.h>

namespace Engine::Helpers {
  /* String helpers */
  auto FindToken(const std::string& str, const std::string& token, std::size_t offset = 0)
      -> std::size_t;
  auto FindTokens(const std::string& str, const std::string& token) -> std::vector< std::size_t >;
  auto GetNextWord(const std::string& str, std::size_t pos, const char separator = ' ') -> std::string;
  auto GetLineFromPosition(const std::string& str, std::size_t pos) -> std::size_t;
  auto GetLineBeginning(const std::string& str, std::size_t line) -> std::size_t;
  auto GetLineEnd(const std::string& str, std::size_t line) -> std::size_t;
}  // namespace Engine::Helpers