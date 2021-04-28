#pragma once
#include <pch.h>
#include <Utility/Helpers.h>
#include <GL/GLEnum.h>

namespace Engine::Utility {
  template< typename EType >
  class BitFlags {
  public:
    typedef typename EType::_integral value_t;
    typedef EType Flag;
    auto Set(const value_t flag) noexcept -> void {
      _state = _state | flag;
    }
    auto Clear(const value_t flag) noexcept -> void {
      _state = _state & ~flag;
    }
    auto Get(const value_t flag) const noexcept -> value_t {
      return _state & flag;
    }
    auto GetAll(const value_t flag) const noexcept -> bool {
      return ((_state & flag) ^ flag) == value_t(0);
    }
    auto GetAny(const value_t flag) const noexcept -> bool {
      return _state & flag != value_t(0);
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
    std::string infoMessage;
    std::vector< std::pair< GL::ShaderType, std::string > > shaders;
  };

  auto ReadTextFile(const std::string_view& fileName) -> std::string;
  auto StripToRelativePath(const std::string& filePath) -> std::string;
  auto ParseShaderSource(std::string source, const std::string& file_path) -> ShaderParseResult;
}  // namespace Engine::Utility

namespace Engine {
  BETTER_ENUM(__DataFlags, uint8_t, Dirty = Helpers::Bit8(0), Updated = Helpers::Bit8(1));
  typedef Utility::BitFlags< __DataFlags > DataFlags;

}  // namespace Engine