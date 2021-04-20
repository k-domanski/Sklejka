#include "pch.h"
#include "GJK.h"

glm::vec3 Engine::Utility::GJK::Shape::center() const {
  return _center;
}

std::vector< glm::vec3 > Engine::Utility::GJK::Shape::vertices() const {
  return _vertices;
}

glm::vec3 Engine::Utility::GJK::Shape::FindFurthestPoint(glm::vec3 direction) const {
  if (_sphere) {
    return _center + (_radius * normalize(direction));
  }

  glm::vec3 maxPoint = _vertices[0];
  float maxDistance  = -FLT_MAX;

  for (glm::vec3 vertex : _vertices) {
    float distance = glm::dot(vertex, direction);
    if (distance > maxDistance) {
      maxDistance = distance;
      maxPoint    = vertex;
    }
  }

  return maxPoint;
}

Engine::Utility::GJK::Simplex&
    Engine::Utility::GJK::Simplex::operator=(std::initializer_list< glm::vec3 > list) {
  for (auto v = list.begin(); v != list.end(); v++) {
    m_points[std::distance(list.begin(), v)] = *v;
  }
  m_size = list.size();

  return *this;
}

void Engine::Utility::GJK::Simplex::push_front(glm::vec3 point) {
  m_points = {point, m_points[0], m_points[1], m_points[2]};
  m_size   = std::min(m_size + 1, 4u);
}

glm::vec3& Engine::Utility::GJK::Simplex::operator[](unsigned i) {
  return m_points[i];
}

unsigned Engine::Utility::GJK::Simplex::size() const {
  return m_size;
}

auto Engine::Utility::GJK::Intersects(Shape s1, Shape s2) -> std::pair< bool, Simplex > {
  // Get initial support point in any direction
  glm::vec3 startDirection(1.0f);
  startDirection    = normalize(startDirection);
  glm::vec3 support = Support(s1, s2, startDirection);
  // Simplex is an array of points, max count is 4
  Simplex points;
  points.push_front(support);

  // New direction is towards the origin
  glm::vec3 direction     = -support;
  unsigned int iterations = 0;
  while (iterations++ < 32) {
    support = Support(s1, s2, direction);

    if (glm::dot(support, direction) <= 0) {
      // if (direction == glm::vec3(0.0f))
      // return std::pair<bool, Simplex>(true, points);
      return std::pair< bool, Simplex >(false, points);  // no collision
    }

    points.push_front(support);
    if (NextSimplex(points, direction)) {
      return std::pair< bool, Simplex >(true, points);
    }
  }

  return std::pair< bool, Simplex >(false, points);
}

auto Engine::Utility::GJK::TripleCrossProduct(glm::vec3 a, glm::vec3 b, glm::vec3 c) -> glm::vec3 {
  auto first  = glm::cross(a, b);
  auto second = glm::cross(first, c);
  return second;
  // return glm::cross(glm::cross(a, b), c);
}

auto Engine::Utility::GJK::Support(Shape s1, Shape s2, glm::vec3 dir) -> glm::vec3 {
  return s1.FindFurthestPoint(dir) - s2.FindFurthestPoint(-dir);
}

bool Engine::Utility::GJK::NextSimplex(Simplex& points, glm::vec3& direction) {
  switch (points.size()) {
    case 2:
      return Line(points, direction);
    case 3:
      return Triangle(points, direction);
    case 4:
      return Tetrahedron(points, direction);
  }

  // never should be here
  return false;
}

bool Engine::Utility::GJK::SameDirection(const glm::vec3& direction, const glm::vec3& ao) {
  return glm::dot(direction, ao) > 0;
}

bool Engine::Utility::GJK::Line(Simplex& points, glm::vec3& direction) {
  glm::vec3 a = points[0];
  glm::vec3 b = points[1];

  glm::vec3 ab = b - a;
  glm::vec3 ao = -a;

  if (SameDirection(ab, ao)) {
    direction = TripleCrossProduct(ab, ao, ab);  // ab.cross(ao).cross(ab);
  }

  else {
    points    = {a};
    direction = ao;
  }

  return false;
}

bool Engine::Utility::GJK::Triangle(Simplex& points, glm::vec3& direction) {
  glm::vec3 a = points[0];
  glm::vec3 b = points[1];
  glm::vec3 c = points[2];

  glm::vec3 ab = b - a;
  glm::vec3 ac = c - a;
  glm::vec3 ao = -a;

  glm::vec3 abc = glm::cross(ab, ac);

  if (SameDirection(glm::cross(abc, ac), ao)) {
    if (SameDirection(ac, ao)) {
      points    = {a, c};
      direction = TripleCrossProduct(ac, ao, ac);  // ac.cross(ao).cross(ac);
    }

    else {
      return Line(points = {a, b}, direction);
    }
  }

  else {
    if (SameDirection(glm::cross(ab, abc), ao)) {
      return Line(points = {a, b}, direction);
    }

    else {
      if (SameDirection(abc, ao)) {
        direction = abc;
      }

      else {
        points    = {a, c, b};
        direction = -abc;
      }
    }
  }

  return false;
}

bool Engine::Utility::GJK::Tetrahedron(Simplex& points, glm::vec3& direction) {
  glm::vec3 a = points[0];
  glm::vec3 b = points[1];
  glm::vec3 c = points[2];
  glm::vec3 d = points[3];

  glm::vec3 ab = b - a;
  glm::vec3 ac = c - a;
  glm::vec3 ad = d - a;
  glm::vec3 ao = -a;

  glm::vec3 abc = glm::cross(ab, ac);
  glm::vec3 acd = glm::cross(ac, ad);
  glm::vec3 adb = glm::cross(ad, ab);

  if (SameDirection(abc, ao)) {
    return Triangle(points = {a, b, c}, direction);
  }

  if (SameDirection(acd, ao)) {
    return Triangle(points = {a, c, d}, direction);
  }

  if (SameDirection(adb, ao)) {
    return Triangle(points = {a, d, b}, direction);
  }

  return true;
}

std::pair< std::vector< glm::vec4 >, size_t >
    Engine::Utility::GJK::GetFaceNormals(const std::vector< glm::vec3 >& polytope,
                                         const std::vector< size_t >& faces) {
  std::vector< glm::vec4 > normals;
  size_t minTriangle = 0;
  float minDistance  = FLT_MAX;

  for (size_t i = 0; i < faces.size(); i += 3) {
    glm::vec3 a = polytope[faces[i]];
    glm::vec3 b = polytope[faces[i + 1]];
    glm::vec3 c = polytope[faces[i + 2]];

    glm::vec3 normal = normalize(glm::cross((b - a), c - a));  //(b - a).cross(c - a).normalized();
    float distance   = glm::dot(normal, a);                    // normal.dot(a);

    if (distance < 0) {
      normal *= -1;
      distance *= -1;
    }

    normals.emplace_back(normal, distance);

    if (distance < minDistance) {
      minTriangle = i / 3;
      minDistance = distance;
    }
  }

  return {normals, minTriangle};
}

void Engine::Utility::GJK::AddIfUniqueEdge(std::vector< std::pair< size_t, size_t > >& edges,
                                           const std::vector< size_t >& faces, size_t a, size_t b) {
  auto reverse = std::find(               //      0--<--3
      edges.begin(),                      //     / \ B /   A: 2-0
      edges.end(),                        //    / A \ /    B: 0-2
      std::make_pair(faces[b], faces[a])  //   1-->--2
  );

  if (reverse != edges.end()) {
    edges.erase(reverse);
  }

  else {
    edges.emplace_back(faces[a], faces[b]);
  }
}
#define EPA_MAX_ITER 32
auto Engine::Utility::GJK::EPA(const Simplex& simplex, const Shape& colliderA,
                               const Shape& colliderB) -> CollisionPoints {
  std::vector< glm::vec3 > polytope(simplex.begin(), simplex.end());
  std::vector< size_t > faces = {0, 1, 2, 0, 3, 1, 0, 2, 3, 1, 3, 2};

  // list: vector4(normal, distance), index: min distance
  auto [normals, minFace] = GetFaceNormals(polytope, faces);
  glm::vec3 minNormal;
  float minDistance = FLT_MAX;

  int iterations = 0;

  while (minDistance == FLT_MAX) {
    minNormal   = glm::make_vec3(normals[minFace]);
    minDistance = normals[minFace].w;

    if (iterations++ > EPA_MAX_ITER) {
      break;
    }

    glm::vec3 support = Support(colliderA, colliderB, minNormal);
    float sDistance   = glm::dot(minNormal, support);  // minNormal.dot(support);

    if (abs(sDistance - minDistance) > 0.001f) {
      minDistance = FLT_MAX;
      std::vector< std::pair< size_t, size_t > > uniqueEdges;

      for (size_t i = 0; i < normals.size(); i++) {
        if (SameDirection(normals[i], support)) {
          size_t f = i * 3;

          AddIfUniqueEdge(uniqueEdges, faces, f, f + 1);
          AddIfUniqueEdge(uniqueEdges, faces, f + 1, f + 2);
          AddIfUniqueEdge(uniqueEdges, faces, f + 2, f);

          faces[f + 2] = faces.back();
          faces.pop_back();
          faces[f + 1] = faces.back();
          faces.pop_back();
          faces[f] = faces.back();
          faces.pop_back();

          normals[i] = normals.back();
          normals.pop_back();

          i--;
        }
      }

      if (uniqueEdges.size() == 0) {
        break;
      }

      std::vector< size_t > newFaces;
      for (auto [edgeIndex1, edgeIndex2] : uniqueEdges) {
        newFaces.push_back(edgeIndex1);
        newFaces.push_back(edgeIndex2);
        newFaces.push_back(polytope.size());
      }

      polytope.push_back(support);

      auto [newNormals, newMinFace] = GetFaceNormals(polytope, newFaces);
      float oldMinDistance          = FLT_MAX;
      for (size_t i = 0; i < normals.size(); i++) {
        if (normals[i].w < oldMinDistance) {
          oldMinDistance = normals[i].w;
          minFace        = i;
        }
      }

      if (newNormals[newMinFace].w < oldMinDistance) {
        minFace = newMinFace + normals.size();
      }

      faces.insert(faces.end(), newFaces.begin(), newFaces.end());
      normals.insert(normals.end(), newNormals.begin(), newNormals.end());
    }
  }

  if (minDistance == FLT_MAX) {
    std::cout << "min distance was inf" << std::endl;
    return {};
  }

  CollisionPoints points;

  points.Normal           = minNormal;
  points.PenetrationDepth = minDistance + 0.001f;
  points.HasCollision     = true;

  return points;
}
