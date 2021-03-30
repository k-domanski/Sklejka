#pragma once
#include <pch.h>
#include <GL/GLEnum.h>

namespace Engine::GL {
  auto GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
                                  GLsizei length, const GLchar* message, const void* userParam)
      -> void;
}