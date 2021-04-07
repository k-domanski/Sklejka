#pragma once
#include <pch.h>

namespace Engine::Utility {
  template<typename EType>
  class BitFlags {
  public:
    typedef typename EType::_integral value_t;
    auto SetFlag(const EType flag) noexcept -> void {
      _state = _state | flag;
    }
    auto ClearFlag(const EType flag) noexcept -> void {
      _state = _state & ~flag;
    }
    auto GetFlag(const EType flag) const noexcept -> bool {
      return (_state & flag) > value_t(0);
    }
    auto ClearAll() noexcept -> void {
      _state = value_t(0);
    }
    auto SetAll() noexcept -> void {
      _state = ~value_t(0);
    }
    auto GetState() const noexcept -> value_t {
      return _state;
    }
  private:
    value_t _state{0};
  };

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