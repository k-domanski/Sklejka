#pragma once
namespace Engine::Utility::GJK {
  class Shape {
  public:
    Shape(std::vector< glm::vec3 > vertices, glm::vec3 center)
        : _vertices(vertices), _center(center) {
      _sphere = false;
      _radius = 0.0f;
    }
    Shape(bool sphere, float radius, glm::vec3 center)
        : _sphere(sphere), _radius(radius), _center(center) {
    }
    Shape() = default;

    ~Shape() = default;

    [[nodiscard]] glm::vec3 center() const;
    __declspec(property(get = center)) glm::vec3 Center;
    [[nodiscard]] std::vector< glm::vec3 > vertices() const;
    __declspec(property(get = vertices)) std::vector< glm::vec3 > Vertices;

    glm::vec3 FindFurthestPoint(glm::vec3 direction) const;

  private:
    glm::vec3 _center;
    std::vector< glm::vec3 > _vertices;
    bool _sphere;
    float _radius;
  };

  struct Simplex {
  private:
    std::array< glm::vec3, 4 > m_points;
    unsigned m_size;

  public:
    Simplex(): m_points({glm::vec3(0), glm::vec3(0), glm::vec3(0), glm::vec3(0)}), m_size(0) {
    }

    Simplex& operator=(std::initializer_list< glm::vec3 > list);

    void push_front(glm::vec3 point);

    glm::vec3& operator[](unsigned i);

    unsigned size() const;

    auto end() const {
      return m_points.end() - (4 - m_size);
    }

    auto begin() const {
      return m_points.begin();
    }
  };

  struct CollisionPoints {
    glm::vec3 A;
    glm::vec3 B;
    glm::vec3 Normal;
    float PenetrationDepth;
    bool HasCollision;
  };

  auto Intersects(Shape s1, Shape s2) -> std::pair< bool, Simplex >;
  auto TripleCrossProduct(glm::vec3 a, glm::vec3 b, glm::vec3 c) -> glm::vec3;
  auto Support(Shape s1, Shape s2, glm::vec3 dir) -> glm::vec3;
  bool NextSimplex(Simplex& points, glm::vec3& direction);

  bool SameDirection(const glm::vec3& direction, const glm::vec3& ao);

  bool Line(Simplex& points, glm::vec3& direction);

  bool Triangle(Simplex& points, glm::vec3& direction);

  bool Tetrahedron(Simplex& points, glm::vec3& direction);

  std::pair< std::vector< glm::vec4 >, size_t >
      GetFaceNormals(const std::vector< glm::vec3 >& polytope, const std::vector< size_t >& faces);

  void AddIfUniqueEdge(std::vector< std::pair< size_t, size_t > >& edges,
                       const std::vector< size_t >& faces, size_t a, size_t b);

  auto EPA(const Simplex& simplex, const Shape& colliderA, const Shape& colliderB)
  -> CollisionPoints;
}  // namespace Engine::Utility::GJK
