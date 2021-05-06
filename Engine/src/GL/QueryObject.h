#pragma once
#include "IGLResource.h"

namespace Engine::GL {
  class QueryObject : public IGLResource {
  public:
    QueryObject() noexcept;
    auto GetHandle() const noexcept -> GLuint override;
    auto Release() -> void;
    auto Start() -> void;
    auto End() -> void;
    auto AnySamplesPassed() -> GLboolean;
  private:
    GLuint _handle;
    bool _started;
  };
}  // namespace Engine::GL
