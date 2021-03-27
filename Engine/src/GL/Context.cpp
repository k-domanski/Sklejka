#include "pch.h"
#include <GL/Context.h>
#include <GL/Buffer.h>

namespace Engine::GL {
  auto Context::VertexBuffer() noexcept -> GLenum {
    return VertexBuffer::GetCurrentHandle();
  }
  auto Context::IndiceBuffer() noexcept -> GLenum {
    return IndiceBuffer::GetCurrentHandle();
  }
}  // namespace Engine::GL
