#include "pch.h"
#include "Helpers.h"

namespace Engine::Helpers {
  auto FindToken(const std::string& str, const std::string& token, std::size_t offset)
      -> std::size_t {
    return str.find(token, offset);
  }
  auto FindTokens(const std::string& str, const std::string& token) -> std::vector< std::size_t > {
    std::vector< std::size_t > positions;
    std::size_t offset{0};
    const auto npos = std::string::npos;
    while (offset != npos) {
      offset = str.find(token, offset);
      if (offset != npos) {
        positions.push_back(offset);
        offset += 1;
      }
    }
    return positions;
  }
  auto GetNextWord(const std::string& str, std::size_t pos, const char separator) -> std::string {
    std::size_t begin = pos;
    std::size_t end   = pos;
    /* Find symbol that is not separator or new line */
    for (auto c = str[pos]; c == separator || c == '\n'; c = str[pos]) {
      if (pos >= str.size())
        return "";
      ++pos;
    }
    begin = pos;
    /* Find either separator or a new line*/
    for (auto c = str[pos]; !(c == separator || c == '\n') && pos < str.size(); c = str[pos]) {
      if (pos >= str.size())
        return str.substr(begin, str.size() - begin);
      ++pos;
    }
    end = pos;
    return str.substr(begin, end - begin);
  }
  auto GetLineFromPosition(const std::string& str, std::size_t pos) -> std::size_t {
    std::size_t offset{0};
    std::size_t line{0};
    while (offset <= pos && offset < str.size()) {
      offset = str.find('\n', offset);
      if (offset != std::string::npos) {
        ++offset;
      }
      ++line;
    }
    return line;
  }
  auto GetLineBeginning(const std::string& str, std::size_t line) -> std::size_t {
    std::size_t pos{0};
    std::size_t last_pos{0};
    std::size_t current_line{0};
    while (pos < str.size() && current_line < line) {
      last_pos = pos;
      pos      = str.find('\n', pos);
      if (pos != std::string::npos) {
        ++pos;
      }
      ++current_line;
    }
    return last_pos;
  }
  auto GetLineEnd(const std::string& str, std::size_t line) -> std::size_t {
    auto begin_pos = GetLineBeginning(str, line);
    auto end_pos   = str.find('\n', begin_pos);
    if (end_pos == std::string::npos) {
      end_pos = str.size() - 1;
    }
    return end_pos;
  }
  auto GetParentFolderPath(const std::string& str) -> std::string {
    auto last_slash = str.find_last_of('/');
    if (last_slash == std::string::npos) {
      return "./";
    }
    return str.substr(0, last_slash + 1);
  }
  auto RemoveAllComments(std::string str, const std::string& commentToken) -> std::string {
    for (auto pos = FindToken(str, commentToken); pos != std::string::npos;
         pos      = FindToken(str, commentToken)) {
      auto line = GetLineFromPosition(str, pos);
      auto end  = GetLineEnd(str, line);
      if (str[end] == '\n') {
        str.erase(pos, end - pos);
      } else {
        str.erase(pos, end - pos + 1);
      }
    }
    return str;
  }
}  // namespace Engine::Helpers