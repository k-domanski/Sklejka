#include "pch.h"
#include "VertexArray.h"

namespace Engine::GL {
  auto VertexArray::GetCurrentHandle() noexcept -> GLuint {
    return s_currentHandle;
  }
  VertexArray::VertexArray() noexcept {
    glGenVertexArrays(1, &_handle);
  }
  VertexArray::VertexArray(VertexArray&& other) noexcept: _handle(other._handle) {
    other._handle = 0;
  }
  auto VertexArray::operator=(VertexArray&& other) noexcept -> VertexArray& {
    if (&other == this)
      return *this;
    Release();
    _handle       = other._handle;
    other._handle = 0;
    return *this;
  }
  auto VertexArray::Bind() noexcept -> void {
    if (IsBound())
      return;
    glBindVertexArray(_handle);
    s_currentHandle = _handle;
  }
  auto VertexArray::IsBound() const noexcept -> bool {
    return s_currentHandle == _handle;
  }
  auto VertexArray::IsValid() const noexcept -> bool {
    return _handle != 0u;
  }
  auto VertexArray::GetHandle() const noexcept -> GLuint {
    return _handle;
  }
  auto VertexArray::Release() -> void {
    glDeleteVertexArrays(1, &_handle);
  }
}  // namespace Engine::GL
