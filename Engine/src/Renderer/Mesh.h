#pragma once
#include <pch.h>
#include <GL/Buffer.h>
#include <GL/VertexArray.h>
#include <GL/Primitives.h>
#include <Renderer/Vertex.h>

namespace Engine::Renderer {
  class Mesh {
  private:
    GL::VertexArray _vertexArray;
    GL::VertexBuffer _vertexBuffer;
    GL::IndiceBuffer _indiceBuffer;
    GL::Primitive _primitive = GL::Primitive::Triangles;
    std::vector< Vertex > _vertexData;
    std::vector< GLuint > _indiceData;

  public:
    Mesh(const std::vector< Vertex >& vertices, const std::vector< GLuint >& indices) noexcept;
    Mesh(const std::vector< Vertex >& vertices, const std::vector< GLuint >& indices,
         GL::Primitive primitive) noexcept;
    Mesh(const Mesh&) = delete;
    auto operator=(const Mesh&) -> Mesh& = delete;
    Mesh(Mesh&& other) noexcept;
    auto operator=(Mesh&& other) -> Mesh&;
    auto Use() noexcept -> void;

  private:
    auto SendDataToBuffers() noexcept -> void;
  };
}  // namespace Engine::Renderer