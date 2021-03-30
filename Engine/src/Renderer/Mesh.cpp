#include "pch.h"
#include "Mesh.h"

namespace Engine::Renderer {
  Mesh::Mesh(const std::vector< Vertex >& vertices, const std::vector< GLuint >& indices) noexcept
      : _vertexData(vertices), _indiceData(indices) {
    SendDataToBuffers();
    Vertex::EnableVertexLayout();
  }
  Mesh::Mesh(const std::vector< Vertex >& vertices, const std::vector< GLuint >& indices,
             GL::Primitive primitive) noexcept
      : Mesh(vertices, indices) {
    _primitive = primitive;
  }
  Mesh::Mesh(Mesh&& other) noexcept
      : _vertexArray(std::move(other._vertexArray)), _vertexBuffer(std::move(other._vertexBuffer)),
        _indiceBuffer(std::move(other._indiceBuffer)), _vertexData(std::move(other._vertexData)),
        _indiceData(std::move(other._indiceData)), _primitive(other._primitive) {
  }
  auto Mesh::operator=(Mesh&& other) -> Mesh& {
    if (&other == this)
      return *this;
    _vertexArray  = std::move(other._vertexArray);
    _vertexBuffer = std::move(other._vertexBuffer);
    _indiceBuffer = std::move(other._indiceBuffer);
    _vertexData   = std::move(other._vertexData);
    _indiceData   = std::move(other._indiceData);
    _primitive    = other._primitive;
    return *this;
  }
  auto Mesh::Use() noexcept -> void {
    _vertexArray.Bind();
    _vertexBuffer.Bind();
    _indiceBuffer.Bind();
  }
  auto Mesh::ElementCount() const noexcept -> GLuint {
    return _indiceData.size();
  }
  auto Mesh::GetPrimitive() const noexcept -> GL::Primitive {
    return _primitive;
  }
  auto Mesh::SendDataToBuffers() noexcept -> void {
    _vertexArray.Bind();
    _vertexBuffer.SetData(_vertexData.size() * sizeof(Vertex), _vertexData.data());
    _indiceBuffer.SetData(_indiceData.size() * sizeof(GLuint), _indiceData.data());
  }
}  // namespace Engine::Renderer
