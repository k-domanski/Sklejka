#include "pch.h"
#include "Mesh.h"
#define _USE_MATH_DEFINES
#include <math.h>

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

  auto Mesh::GetVertices() const -> std::vector< Vertex > {
    return _vertexData;
  }

  auto Mesh::SendDataToBuffers() noexcept -> void {
    _vertexArray.Bind();
    _vertexBuffer.SetData(_vertexData.size() * sizeof(Vertex), _vertexData.data());
    _indiceBuffer.SetData(_indiceData.size() * sizeof(GLuint), _indiceData.data());
  }
  auto Mesh::GetPrimitive(MeshPrimitive primitive) noexcept -> std::shared_ptr< Mesh > {
    if (_primitiveMeshes.count(primitive) == 0) {
      _primitiveMeshes[primitive] = CreatePrimitive(primitive);
    }
    return _primitiveMeshes[primitive];
  }
  auto Mesh::CreatePrimitive(MeshPrimitive primitive) noexcept -> std::shared_ptr< Mesh > {
    switch (primitive) {
      case MeshPrimitive::Plane: {
        return CreatePlane();
      }
      case MeshPrimitive::WireframeBox: {
        return CreateWireframeBox();
      }
      case MeshPrimitive::WireframeSphere: {
        return CreateWireframeSphere();
      }
    }
    LOG_WARN("No matching mesh primitive function: Primitive [{}]", primitive._to_string());
    return nullptr;
  }
  auto Mesh::CreatePlane() noexcept -> std::shared_ptr< Mesh > {
    const std::vector< Vertex > verts{{// Vert 0
                                       {-1.0f, -1.0f, 0.0f},
                                       {0.0f, 0.0f, 1.0f},
                                       {0.0f, 0.0f}},
                                      {// Vert 1
                                       {1.0f, -1.0f, 0.0f},
                                       {0.0f, 0.0f, 1.0f},
                                       {1.0f, 0.0f}},
                                      {// Vert 2
                                       {1.0f, 1.0f, 0.0f},
                                       {0.0f, 0.0f, 1.0f},
                                       {1.0f, 1.0f}},
                                      {// Vert 3
                                       {-1.0f, 1.0f, 0.0f},
                                       {0.0f, 0.0f, 1.0f},
                                       {0.0f, 1.0f}}};
    const std::vector< GLuint > inds{0, 1, 2, 2, 3, 0};
    return std::make_shared< Mesh >(verts, inds);
  }

  auto Mesh::CreateWireframeBox() noexcept -> std::shared_ptr< Mesh > {
    const std::vector< Vertex > verts{
        {// Vert 0
         {-0.5f, -0.5f, -0.5f},
         {0.0f, 0.0f, 0.0f},
         {0.0f, 0.0f}},
        {// Vert 1
         {-0.5f, -0.5f, 0.5f},
         {0.0f, 0.0f, 0.0f},
         {0.0f, 0.0f}},
        {// Vert 2
         {-0.5f, 0.5f, 0.5f},
         {0.0f, 0.0f, 0.0f},
         {0.0f, 0.0f}},
        {// Vert 3
         {-0.5f, 0.5f, -0.5f},
         {0.0f, 0.0f, 0.0f},
         {0.0f, 0.0f}},
        {// Vert 4
         {0.5f, -0.5f, -0.5f},
         {0.0f, 0.0f, 0.0f},
         {0.0f, 0.0f}},
        {// Vert 5
         {0.5f, -0.5f, 0.5f},
         {0.0f, 0.0f, 0.0f},
         {0.0f, 0.0f}},
        {// Vert 6
         {0.5f, 0.5f, 0.5f},
         {0.0f, 0.0f, 0.0f},
         {0.0f, 0.0f}},
        {// Vert 7
         {0.5f, 0.5f, -0.5f},
         {0.0f, 0.0f, 0.0f},
         {0.0f, 0.0f}},
    };

    const std::vector< GLuint > inds{0, 1, 1, 2, 2, 3, 3, 0, 0, 4, 1, 5,
                                     2, 6, 3, 7, 4, 5, 5, 6, 6, 7, 7, 4};
    return std::make_shared< Mesh >(verts, inds, GL::Primitive::Lines);
  }

  auto Mesh::CreateWireframeSphere() noexcept -> std::shared_ptr< Mesh > {
    const float sphereLOD = 30.0f;
    std::vector< Vertex > verts;
    std::vector< GLuint > inds;

    glm::vec3 normal(0.0f);
    glm::vec3 uv(0.0f);

    float theta = 0.0f;
    int j       = 0;

    for (int i = 0; i < sphereLOD; ++i) {
      Vertex vert{glm::vec3(cos(theta), sin(theta), 0.0f), normal, uv};
      verts.push_back(vert);
      inds.push_back(j++);

      theta += 2.0f * M_PI / sphereLOD;
    }
    theta = 0.0f;
    inds.push_back(2137);
    for (int i = 0; i < sphereLOD; ++i) {
      Vertex vert{glm::vec3(0.0f, sin(theta), cos(theta)), normal, uv};
      verts.push_back(vert);
      inds.push_back(j++);

      theta += 2.0f * M_PI / sphereLOD;
    }
    inds.push_back(2137);
    theta = 0.0f;
    for (int i = 0; i < sphereLOD; ++i) {
      Vertex vert{glm::vec3(cos(theta), 0.0f, sin(theta)), normal, uv};
      verts.push_back(vert);
      inds.push_back(j++);

      theta += 2.0f * M_PI / sphereLOD;
    }

    return std::make_shared< Mesh >(verts, inds, GL::Primitive::LineLoop);
  }
}  // namespace Engine::Renderer
