#include "pch.h"
#include "QueryObject.h"

Engine::GL::QueryObject::QueryObject() noexcept {
  glGenQueries(1, &_handle);
  _queryType = GL_ANY_SAMPLES_PASSED;
  _started = false;
}

auto Engine::GL::QueryObject::GetHandle() const noexcept -> GLuint {
  return _handle;
}

auto Engine::GL::QueryObject::Release() -> void {
  glDeleteQueries(1, &_handle);
}

auto Engine::GL::QueryObject::Start() -> void {
  glBeginQuery(_queryType, _handle);
  _started = true;
}

auto Engine::GL::QueryObject::Start(GLint queryType) -> void
{
  _queryType = queryType;
  glBeginQuery(_queryType, _handle);
  _started = true;
}

auto Engine::GL::QueryObject::End() -> void {
  glEndQuery(_queryType);
}

auto Engine::GL::QueryObject::AnySamplesPassed() -> GLboolean {
  GLint res;
  if (!_started)
    return false;
  glGetQueryObjectiv(_handle, GL_QUERY_RESULT, &res);
  //std::cout << res << std::endl;
  return res > 0 ? GL_TRUE : GL_FALSE;
}

auto Engine::GL::QueryObject::SamplesPassed() -> GLint
{
  GLint res;
  if (!_started)
    return 0;
  glGetQueryObjectiv(_handle, GL_QUERY_RESULT, &res);
  // std::cout << res << std::endl;
  return res;
}
