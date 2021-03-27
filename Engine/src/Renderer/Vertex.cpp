#include "pch.h"
#include "Vertex.h"
#include <cstddef>

namespace Engine::Renderer {
  auto Vertex::EnableVertexLayout() -> void {
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast< const void* >(offsetof(Vertex, position)));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast< const void* >(offsetof(Vertex, normal)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast< const void* >(offsetof(Vertex, uv)));
  }
}  // namespace Engine::Renderer
