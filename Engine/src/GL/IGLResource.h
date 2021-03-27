#pragma once
#include<pch.h>

namespace Engine::GL {
  class IGLResource{
  public:
    virtual auto GetHandle() const noexcept -> GLuint = 0;
    virtual ~IGLResource() = default;
  };
}