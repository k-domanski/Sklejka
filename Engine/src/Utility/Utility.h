#pragma once
#include <pch.h>
#include <Utility/Helpers.h>
#include <GL/GLEnum.h>

#include "GL/Texture2D.h"
#define READ_VALUE(json, attribute_name, default_value)                                            \
  (json.count(attribute_name) == 0 ? default_value : json[attribute_name])

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
    auto SetState(value_t state) noexcept -> void {
      _state = state;
    }
    auto GetState() const noexcept -> value_t {
      return _state;
    }

  private:
    value_t _state{0};
  };

  template< typename T >
  class TimeLerp {
  private:
    T _start, _end;
    float _duration, _time{0.0f};

  public:
    TimeLerp() = default;
    TimeLerp(T start, T end, float duration)
        : _start(start), _end(end), _duration(duration), _time(0.0f) {
    }
    auto Set(T start, T end, float duration) {
      _start    = start;
      _end      = end;
      _duration = duration;
      _time     = 0.0f;
    }
    auto Update(float deltaTime) -> T {
      auto t = glm::clamp(_time / _duration, 0.0f, 1.0f);
      _time += deltaTime;
      return (1.0f - t) * _start + t * _end;
    }
    auto Reset() -> void {
      _time = 0.0f;
    }
    auto Finished() -> bool {
      return (_duration - _time) <= 0.0f;
    }
  };

  struct ShaderParseResult {
    bool success = false;
    std::string version;
    std::string infoMessage;
    std::vector< std::pair< GL::ShaderType, std::string > > shaders;
  };

  struct Character {
    std::shared_ptr< GL::Texture2D > Texture;
    glm::ivec2 Size;
    glm::ivec2 Bearing;
    unsigned int Advance;
  };

  auto InRange(int value, int min, int max) -> bool;
  auto ReadTextFile(const std::string_view& fileName, bool* success = nullptr) -> std::string;
  auto StripToRelativePath(const std::string& filePath) -> std::string;
  auto ParseShaderSource(std::string source, const std::string& file_path) -> ShaderParseResult;
}  // namespace Engine::Utility

namespace Engine {
  BETTER_ENUM(__DataFlags, uint8_t, Dirty = Helpers::Bit8(0), Updated = Helpers::Bit8(1));
  typedef Utility::BitFlags< __DataFlags > DataFlags;

}  // namespace Engine