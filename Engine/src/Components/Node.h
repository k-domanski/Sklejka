#pragma once
#include <ECS/Component.h>

namespace Engine {
  class Node : public ECS::Component {
  private:
    int _index{0};
    int _nextIndex{0};
    /*
     Add other params if needed
     i.e.: range of player movement for the current segment, speed, etc
    */

  public:
    Node();
    auto Index() const noexcept -> int;
    auto Index(int index) noexcept -> int;
    auto NextIndex() const noexcept -> int;
    auto NextIndex(int index) noexcept -> int;
    auto SaveToJson() -> std::string override;
    auto LoadFromJson(std::string filePath) -> void override;
  };
}  // namespace Engine