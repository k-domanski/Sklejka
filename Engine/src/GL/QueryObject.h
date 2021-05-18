#pragma once
#include "IGLResource.h"

namespace Engine::GL {
  class QueryObject : public IGLResource {
  public:
    QueryObject() noexcept;
    auto GetHandle() const noexcept -> GLuint override;
    auto Release() -> void;
    auto Start() -> void;
    auto Start(GLint queryType) -> void;
    auto End() -> void;
    auto AnySamplesPassed() -> GLboolean;
    auto SamplesPassed() -> GLint;
  private:
    GLuint _handle;
    bool _started;
    GLint _queryType;
  };
}  // namespace Engine::GL
