#pragma once
#include<pch.h>

namespace Engine::GL {
  class IGLResource{
  public:
    virtual GLuint GetHandle() const = 0;
    virtual ~IGLResource() = default;
  };
}