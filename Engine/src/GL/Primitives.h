#pragma once
#include <pch.h>

namespace Engine::GL {
  BETTER_ENUM(__Primitive, GLenum, Points = GL_POINTS, LineStrip = GL_LINE_STRIP, Lines = GL_LINES,
              Triangles = GL_TRIANGLES, TriangleStrip = GL_TRIANGLE_STRIP);

  typedef __Primitive Primitive;
}