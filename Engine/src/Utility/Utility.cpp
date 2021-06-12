#include "pch.h"
#include "Utility.h"
#include <Utility/Helpers.h>
#include <GL/GLEnum.h>
#include <App/Log.h>
#include <filesystem>

namespace fs = std::filesystem;

auto RecursiveIncludeProcess(std::string& content, const std::string& parent_dir,
                             std::vector< std::string >& included_files) -> void {
  using namespace Engine::Helpers;
  using namespace Engine::Utility;

  const std::string tag = "#include";
  const auto tag_len    = tag.size();

  const auto include_pos = FindTokens(content, tag);
  int include_count      = 0;

  for (int i = include_pos.size() - 1; i >= 0; --i) {
    auto fp = GetNextWord(content, include_pos[i] + tag_len);
    fp      = fp.substr(1, fp.size() - 2);

    auto line_end = GetLineEnd(content, GetLineFromPosition(content, include_pos[i]));
    content.erase(include_pos[i], line_end - include_pos[i] + 1);

    auto it = std::find(included_files.begin(), included_files.end(), fp);
    if (it != included_files.end()) {
      // Already included
      continue;
    }

    included_files.push_back(fp);
    auto include_content = ReadTextFile(parent_dir + fp);
    include_content      = RemoveAllComments(include_content, "//");
    content.insert(include_pos[i], "\n" + include_content);

    ++include_count;
  }

  if (include_count == 0)
    return;

  RecursiveIncludeProcess(content, parent_dir, included_files);
}

auto ProcessIncludeDirective(std::string content, const std::string& parent_dir) -> std::string {
  using namespace Engine::Helpers;
  using namespace Engine::Utility;

  std::vector< std::string > included_files;
  RecursiveIncludeProcess(content, parent_dir, included_files);

  return content;
}

namespace Engine::Utility {
  QuatLerp::QuatLerp(glm::quat start, glm::quat end, float duration)
      : _start(start), _end(end), _duration(duration), _time(0.0f) {
  }
  auto QuatLerp::Set(glm::quat start, glm::quat end, float duration) -> void {
    _start    = start;
    _end      = end;
    _value    = start;
    _duration = duration;
    _time     = 0.0f;
  }
  auto QuatLerp::Update(float deltaTime) -> glm::quat {
    auto t = glm::clamp(_time / _duration, 0.0f, 1.0f);
    _time += deltaTime;
    _value = glm::slerp(_start, _end, t);
    return _value;
  }
  auto QuatLerp::Reset() -> void {
    _time = 0.0f;
  }
  auto QuatLerp::Finished() -> bool {
    return (_duration - _time) <= 0.0f;
  }
  auto QuatLerp::Start() -> glm::quat {
    return _start;
  }
  auto QuatLerp::End() -> glm::quat {
    return _end;
  }
  auto QuatLerp::Value() -> glm::quat {
    return _value;
  }
}  // namespace Engine::Utility

auto Engine::Utility::InRange(int value, int min, int max) -> bool {
  return (value >= min) && (value <= max);
}

auto Engine::Utility::ReadTextFile(const std::string_view& fileName, bool* success) -> std::string {
  std::ifstream file(fileName.data());
  if (!file.good()) {
    LOG_ERROR("Failed to open the file: {}", fileName);
    if (success != nullptr) {
      *success = false;
    }
    return "";
  }

  std::string content((std::istreambuf_iterator< char >(file)), std::istreambuf_iterator< char >());
  if (success != nullptr) {
    *success = true;
  }
  return content;
}

auto Engine::Utility::StripToRelativePath(const std::string& filePath) -> std::string {
  auto relative_path = filePath;
  auto working_dir   = fs::current_path().string();
  auto pos           = relative_path.find(working_dir);
  if (pos != std::string::npos) {
    relative_path.erase(0, working_dir.size());
    relative_path.insert(0, "./");
  }
  return relative_path;
}

auto Engine::Utility::ParseShaderSource(std::string source, const std::string& file_path)
    -> ShaderParseResult {
  // Search for #version
  using namespace Helpers;
  const std::string_view default_version("#version 430");
  // const std::string parentDirectory = GetParentFolderPath(file_path);
  const std::string parentDirectory = fs::path(file_path).parent_path().string() + "/";
  ShaderParseResult result;
  result.success = false;

  /* Remove all comments */
  source = RemoveAllComments(source, "//");

  /* Find version */
  std::string version;
  const auto version_tag_len = std::string_view("#version").size();
  auto versionPos            = FindTokens(source, "#version");
  if (versionPos.size() > 1) {
    // TODO: Log too many versions, pick the first one
    result.infoMessage = "Multiple #version declarations - picked first";
  }
  if (versionPos.size() == 0) {
    // TODO: Log missing #version, assume 430
    version            = default_version;
    result.version     = "430";
    result.infoMessage = "Missing #version declaration - assuming 430";
  } else {
    auto vpos      = versionPos[0];
    auto vnumber   = GetNextWord(source, vpos + version_tag_len);
    version        = std::string("#version ") + vnumber + "\n";
    result.version = vnumber;
  }

  /* Find shaders */
  const auto begin_tag_len = std::string_view("#shader").size();
  const auto end_tag_len   = std::string_view("#endshader").size();
  const auto sh_begin      = FindTokens(source, "#shader");
  const auto sh_end        = FindTokens(source, "#endshader");
  const auto sh_count      = std::min(sh_begin.size(), sh_end.size());
  /* if sh_begin.size() != sh_end.size() then there is missing tag
    Cases:
    1. Missing sh_begin -> begin.size() < end.size()
    2. Missing sh_end -> begin.size() > end.size()
  */
  if (sh_begin.size() < sh_end.size()) {
    // TODO: Log missmatch and find line of the sh_end from pair
    std::size_t index = 0;
    while (index < sh_count && sh_begin[index] < sh_end[index]) {
      ++index;
    }
    auto line          = GetLineFromPosition(source, sh_end[index]);
    result.infoMessage = "Missing #shader tag for closing tag on line " + std::to_string(line);
    return result;
  }
  if (sh_begin.size() > sh_end.size()) {
    std::size_t index = 0;
    while (index < sh_count && sh_begin[index] < sh_end[index]) {
      ++index;
    }
    auto line          = GetLineFromPosition(source, sh_begin[index]);
    result.infoMessage = "Missing #endshader tag for opening tag on line " + std::to_string(line);
    return result;
  }
  /* if sh_begin.size() == sh_end.size() then
    Case:
    1. begin.sh_count == 0 -> error, no tags
    2. sh_count > 0 -> good so far
  */
  if (sh_count == 0) {
    result.infoMessage = "Missing #shader and #endshader tags";
    return result;
  }

  /* Check for tag order -> begin tag < end tag*/

  // Find shader types
  std::vector< GL::ShaderType > sh_types;
  for (std::size_t index = 0; index < sh_count; ++index) {
    auto type_str = GetNextWord(source, sh_begin[index] + begin_tag_len);
    if (type_str == "vertex") {
      sh_types.emplace_back(GL::ShaderType::VertexShader);
    } else if (type_str == "geometry") {
      sh_types.emplace_back(GL::ShaderType::GeometryShader);
    } else if (type_str == "fragment") {
      sh_types.emplace_back(GL::ShaderType::FragmentShader);
    } else {
      // Default case - invalid type
      result.infoMessage = "Invalid shader type: " + type_str;
      return result;
    }
  }

  // Prepare sub shaders
  for (std::size_t index = 0; index < sh_count; ++index) {
    // Extract content
    auto begin_line = GetLineFromPosition(source, sh_begin[index]);
    auto end_line   = GetLineFromPosition(source, sh_end[index]);
    auto begin_pos  = GetLineBeginning(source, begin_line + 1);
    auto end_pos    = GetLineEnd(source, end_line - 1);
    auto content    = source.substr(begin_pos, end_pos - begin_pos + 1);

    // Parse #include in extracted content
    // auto parent_dir = GetParentFolderPath(file_path);
    content = ProcessIncludeDirective(content, parentDirectory);

    // Inject version
    content.insert(0, version);
    result.shaders.emplace_back(sh_types[index], content);
  }
  // Finished
  result.success = true;
  return result;
}
