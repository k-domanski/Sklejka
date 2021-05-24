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
    glVertexAttribIPointer(3, 4, GL_INT, sizeof(Vertex),
        reinterpret_cast<const void*>(offsetof(Vertex, jointIDs)));
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex),
        reinterpret_cast<const void*>(offsetof(Vertex, weights)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(4);
  }
}  // namespace Engine::Renderer
