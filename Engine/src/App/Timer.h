#pragma once
#include <pch.h>

namespace Engine {
  class Timer {
  private:
    double _last  = 0.0;
    double _now   = 0.0;
    double _delta = 0.0;

  public:
    auto Update() -> void {
      auto current = glfwGetTime();
      _delta       = current - _last;
      _last        = _now;
      _now         = current;
    }
    auto DeltaTime() -> double {
      return _delta;
    }
  };
}  // namespace Engine