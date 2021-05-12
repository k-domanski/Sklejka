#include "pch.h"
#include "Node.h"
#include <nlohmann/json.hpp>
#include <Utility/Utility.h>

namespace Engine {

  Node::Node(): ECS::Component("Node"), _index(0), _nextIndex(0) {
  }
  auto Node::Index() const noexcept -> int {
    return _index;
  }
  auto Node::Index(int index) noexcept -> int {
    return _index = index;
  }
  auto Node::NextIndex() const noexcept -> int {
    return _nextIndex;
  }
  auto Node::NextIndex(int index) noexcept -> int {
    return _nextIndex = index;
  }
  auto Node::SaveToJson() -> std::string {
    nlohmann::json json =
        nlohmann::json{{"componentType", "node"}, {"index", _index}, {"nextIndex", _nextIndex}};
    return json.dump(4);
  }
  auto Node::LoadFromJson(std::string filePath) -> void {
    nlohmann::json json;
    if (filePath[0] == '{' || filePath[0] == '\n')  // HACK: Check if string is json
      json = nlohmann::json::parse(filePath.begin(), filePath.end());
    else {
      auto content = Utility::ReadTextFile(filePath);
      json         = nlohmann::json::parse(content.begin(), content.end());
    }

    Index(int(json["index"]));
    NextIndex(int(json["nextIndex"]));
  }
}  // namespace Engine
