#pragma once
#include <pch.h>
#include <GL/Buffer.h>
#include <GL/VertexArray.h>
#include <GL/GLEnum.h>
#include <Renderer/Vertex.h>

namespace Engine::Renderer {
  BETTER_ENUM(__MeshPrimitive, uint8_t, Plane, WireframeBox, WireframeSphere);
  typedef __MeshPrimitive MeshPrimitive;

  class Mesh {
  private:
    static inline std::unordered_map< MeshPrimitive::_integral, std::shared_ptr< Mesh > >
        _primitiveMeshes;
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
    auto ElementCount() const noexcept -> GLuint;
    auto GetPrimitive() const noexcept -> GL::Primitive;
    auto GetVertices() const -> std::vector< Vertex >;

  private:
    auto SendDataToBuffers() noexcept -> void;
    static auto CreatePrimitive(MeshPrimitive primitive) noexcept -> std::shared_ptr< Mesh >;
    static auto CreatePlane() noexcept -> std::shared_ptr< Mesh >;
    static auto CreateWireframeBox() noexcept -> std::shared_ptr< Mesh >;
    static auto CreateWireframeSphere() noexcept -> std::shared_ptr< Mesh >;

  public:
    static auto GetPrimitive(MeshPrimitive primitive) noexcept -> std::shared_ptr< Mesh >;
  };
}  // namespace Engine::Renderer