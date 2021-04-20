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
  float maxDistance = -FLT_MAX;

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

auto Engine::Utility::GJK::Intersects(Shape s1, Shape s2) -> bool {
  // Get initial support point in any direction
  glm::vec3 support = Support(s1, s2, glm::vec3(1.0f, 0.0f, 0.0f));
  // Simplex is an array of points, max count is 4
  Simplex points;
  points.push_front(support);

  // New direction is towards the origin
  glm::vec3 direction = -support;

  while (true) {
    support = Support(s1, s2, direction);

    if (glm::dot(support, direction) <= 0) {
      return false;  // no collision
    }

    points.push_front(support);
    if (NextSimplex(points, direction)) {
      return true;
    }
  }
}

auto Engine::Utility::GJK::TripleCrossProduct(glm::vec3 a, glm::vec3 b, glm::vec3 c) -> glm::vec3 {
  auto first = glm::cross(a, b);
  auto second = glm::cross(first, c);
  return second;
  //return glm::cross(glm::cross(a, b), c);
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

auto Engine::Utility::GJK::Simplex::end() const {
  return m_points.end() - (4 - m_size);
}

auto Engine::Utility::GJK::Simplex::begin() const {
  return m_points.begin();
}
