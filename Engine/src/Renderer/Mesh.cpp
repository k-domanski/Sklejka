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
    _name      = "mesh";
    _parent    = -1;
  }
  Mesh::Mesh(Mesh&& other) noexcept
      : _vertexArray(std::move(other._vertexArray)), _vertexBuffer(std::move(other._vertexBuffer)),
        _indiceBuffer(std::move(other._indiceBuffer)), _vertexData(std::move(other._vertexData)),
        _indiceData(std::move(other._indiceData)), _primitive(other._primitive),
        _parent(other._parent) {
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
    _parent       = other._parent;
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

  auto Mesh::GetName() -> std::string {
    return _name;
  }

  auto Mesh::SetName(std::string name) -> void {
    _name = name;
  }

  auto Mesh::GetParentMesh() -> int {
    return _parent;
  }

  auto Mesh::SetParentMesh(int parentMeshIndex) -> void {
    _parent = parentMeshIndex;
  }

  auto Mesh::GetModelMatrix() -> glm::mat4 {
    return _modelMatrix;
  }

  auto Mesh::SetModelMatrix(glm::mat4 matrix) -> void {
    _modelMatrix = matrix;
  }

  void Mesh::CreateBoundingSphere() {
    _sphereCenter     = glm::vec3(0.0f);  // for now its enough latter maybe average vertex?
    float maxDistance = 0;

    for (auto vertex : _vertexData) {
      _sphereCenter += vertex.position;
    }
    _sphereCenter /= _vertexData.size();

    for (auto vertex : _vertexData) {
      float distance = glm::distance(vertex.position, _sphereCenter);
      if (distance > maxDistance)
        maxDistance = distance;
    }

    _radius = maxDistance;
  }

  void Mesh::CreateBoundingBox() {
    glm::vec3 min(INFINITY);
    glm::vec3 max(-INFINITY);

    for (auto& vertex : _vertexData) {
      if (vertex.position.x > max.x)
        max.x = vertex.position.x;
      if (vertex.position.y > max.y)
        max.y = vertex.position.y;
      if (vertex.position.z > max.z)
        max.z = vertex.position.z;
      if (vertex.position.x < min.x)
        min.x = vertex.position.x;
      if (vertex.position.y < min.y)
        min.y = vertex.position.y;
      if (vertex.position.z < min.z)
        min.z = vertex.position.z;
    }

    const std::vector< Vertex > verts{{// Vert 0
                                       {min.x, min.y, min.z},
                                       {0.0f, 0.0f, 0.0f},
                                       {0.0f, 0.0f}},
                                      {// Vert 1
                                       {min.x, max.y, min.z},
                                       {0.0f, 0.0f, 0.0f},
                                       {0.0f, 0.0f}},
                                      {// Vert 2
                                       {min.x, max.y, max.z},
                                       {0.0f, 0.0f, 0.0f},
                                       {0.0f, 0.0f}},
                                      {// Vert 3
                                       {min.x, min.y, max.z},
                                       {0.0f, 0.0f, 0.0f},
                                       {0.0f, 0.0f}},
                                      {// Vert 4
                                       {max.x, min.y, min.z},
                                       {0.0f, 0.0f, 0.0f},
                                       {0.0f, 0.0f}},
                                      {// Vert 5
                                       {max.x, max.y, min.z},
                                       {0.0f, 0.0f, 0.0f},
                                       {0.0f, 0.0f}},
                                      {// Vert 6
                                       {max.x, max.y, max.z},
                                       {0.0f, 0.0f, 0.0f},
                                       {0.0f, 0.0f}},
                                      {// Vert 7
                                       {max.x, min.y, max.z},
                                       {0.0f, 0.0f, 0.0f},
                                       {0.0f, 0.0f}}};
    const std::vector< GLuint > inds{0, 3, 2, 2, 1, 0, 3, 7, 6, 6, 2, 3, 7, 4, 5, 5, 6, 7,
                                     4, 0, 1, 1, 5, 4, 1, 2, 6, 6, 5, 1, 4, 7, 3, 3, 0, 4};
    _boundingBox = std::make_shared< Mesh >(verts, inds);
  }

  std::pair< glm::vec3, float > Mesh::GetBoundingSphere() {
    return std::make_pair(_sphereCenter, _radius);
  }

  std::shared_ptr< Mesh > Mesh::GetBoundingBox() {
    return _boundingBox;
  }

  auto Mesh::GetQuery() -> GL::QueryObject& {
    return _query;
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
      case MeshPrimitive::Cube: {
        return CreateCube();
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
  auto Mesh::CreateCube() noexcept -> std::shared_ptr< Mesh > {
    const std::vector< Vertex > verts{{// Vert 0
                                       {-.5f, -.5f, .5f},
                                       {0.0f, 0.0f, 0.0f},
                                       {0.0f, 0.0f}},
                                      {// Vert 1
                                       {.5f, -.5f, .5f},
                                       {0.0f, 0.0f, 0.0f},
                                       {0.0f, 0.0f}},
                                      {// Vert 2
                                       {.5f, .5f, .5f},
                                       {0.0f, 0.0f, 0.0f},
                                       {0.0f, 0.0f}},
                                      {// Vert 3
                                       {-.5f, .5f, .5f},
                                       {0.0f, 0.0f, 0.0f},
                                       {0.0f, 0.0f}},
                                      {// Vert 4
                                       {-.5f, -.5f, -.5f},
                                       {0.0f, 0.0f, 0.0f},
                                       {0.0f, 0.0f}},
                                      {// Vert 5
                                       {.5f, -.5f, -.5f},
                                       {0.0f, 0.0f, 0.0f},
                                       {0.0f, 0.0f}},
                                      {// Vert 6
                                       {.5f, .5f, -.5f},
                                       {0.0f, 0.0f, 0.0f},
                                       {0.0f, 0.0f}},
                                      {// Vert 7
                                       {-.5f, .5f, -.5f},
                                       {0.0f, 0.0f, 0.0f},
                                       {0.0f, 0.0f}}};
    const std::vector< GLuint > inds{0, 1, 2, 2, 3, 0, 1, 5, 6, 6, 2, 1, 5, 4, 7, 7, 6, 5,
                                     4, 0, 3, 3, 7, 4, 3, 2, 6, 6, 7, 3, 4, 5, 1, 1, 0, 4};
    return std::make_shared< Mesh >(verts, inds);
  }
}  // namespace Engine::Renderer
