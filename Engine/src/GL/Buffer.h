#pragma once
#include <GL/UniformBufferData.h>

namespace Engine::GL {
  template< GLenum BufferType >
  class Buffer {
  private:
    static inline GLuint s_currentHandle = 0u;
    GLuint _handle;

  public:
    static auto GetCurrentHandle() noexcept -> GLuint {
      return s_currentHandle;
    }
    Buffer() noexcept {
      glGenBuffers(1, &_handle);
    }
    Buffer(Buffer< BufferType >&& other) noexcept: _handle(other._handle) {
      other._handle = 0u;
    }
    auto operator=(Buffer< BufferType >&& other) noexcept -> Buffer< BufferType >& {
      if (&other == this)
        return *this;
      Release();
      _handle       = other._handle;
      other._handle = 0u;
      return *this;
    }
    auto GetHandle() const noexcept -> GLuint {
      return _handle;
    }
    auto Bind() noexcept -> void {
      if (IsBound())
        return;
      glBindBuffer(BufferType, _handle);
      s_currentHandle = _handle;
    }
    auto IsBound() const noexcept -> bool {
      return _handle == s_currentHandle;
    }
    auto IsValid() const noexcept -> bool {
      return _handle != 0u;
    }
    auto SetData(GLsizeiptr size, const void* data, GLenum usage = GL_STATIC_DRAW) noexcept
        -> void {
      if (!IsValid())
        return;
      Bind();
      glBufferData(BufferType, size, data, usage);
    }
    constexpr auto GetBufferType() const noexcept -> GLenum {
      return BufferType;
    }

  private:
    auto Release() noexcept -> void {
      glDeleteBuffers(1, &_handle);
      _handle = 0;
    }
  };

  /* GL Buffers */
  typedef Buffer< GL_ARRAY_BUFFER > VertexBuffer;
  typedef Buffer< GL_ELEMENT_ARRAY_BUFFER > IndiceBuffer;

  template< typename DataType >
  class UniformBuffer : public Buffer< GL_UNIFORM_BUFFER > {
  private:
    static inline std::unordered_map< GLuint, GLuint > s_boundSlots;

  public:
    UniformBuffer() {
      Buffer::SetData(sizeof(DataType), NULL);
      static_assert(std::is_standard_layout_v< DataType >, "DataType has to be standard layout");
    }
    auto SetData(const DataType& data, GLenum usage = GL_STATIC_DRAW) noexcept -> void {
      Buffer::SetData(sizeof(DataType), static_cast< const void* >(&data), usage);
    }
    auto BindToSlot(GLuint slot) noexcept -> void {
      if (s_boundSlots.count(slot) != 0) {
        if (s_boundSlots[slot] == GetHandle())
          return;
      }
      s_boundSlots[slot] = GetHandle();
      glBindBufferBase(GL_UNIFORM_BUFFER, slot, GetHandle());
    }
    auto GetBoundSlot() noexcept -> GLint {
      auto it = std::find_if(s_boundSlots.begin(), s_boundSlots.end(),
                             [auto handle = GetHandle()](auto& kv) { return kv.second == handle; });
      if (it == s_boundSlots.end()) {
        return -1;
      }
      return *it;
    }
  };

  typedef UniformBuffer< CameraUniformData > CameraUniformBuffer;
  typedef UniformBuffer< TransformUniformData > TransformUniformBuffer;
}  // namespace Engine::GL