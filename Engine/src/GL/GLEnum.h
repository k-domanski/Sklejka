#pragma once
#include <pch.h>

namespace Engine::GL {
  BETTER_ENUM(__ShaderType, GLenum, VertexShader = GL_VERTEX_SHADER,
              FragmentShader = GL_FRAGMENT_SHADER);
  typedef __ShaderType ShaderType;

  BETTER_ENUM(__Primitive, GLenum, Points = GL_POINTS, LineStrip = GL_LINE_STRIP, Lines = GL_LINES,
              Triangles = GL_TRIANGLES, TriangleStrip = GL_TRIANGLE_STRIP, LineLoop = GL_LINE_LOOP);
  typedef __Primitive Primitive;

  BETTER_ENUM(__MessageSource, GLenum, API = GL_DEBUG_SOURCE_API,
              WindowSystem   = GL_DEBUG_SOURCE_WINDOW_SYSTEM,
              ShaderCompiler = GL_DEBUG_SOURCE_SHADER_COMPILER,
              ThirdParty = GL_DEBUG_SOURCE_THIRD_PARTY, Application = GL_DEBUG_SOURCE_APPLICATION,
              Other = GL_DEBUG_SOURCE_OTHER);
  typedef __MessageSource MessageSource;

  BETTER_ENUM(__MessageType, GLenum, Error = GL_DEBUG_TYPE_ERROR,
              DeprecatedBehavior = GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR,
              UndefinedBehavior  = GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR,
              Pportability = GL_DEBUG_TYPE_PORTABILITY, Performance = GL_DEBUG_TYPE_PERFORMANCE,
              Marker = GL_DEBUG_TYPE_MARKER, PushGroup = GL_DEBUG_TYPE_PUSH_GROUP,
              PopGroup = GL_DEBUG_TYPE_POP_GROUP, Other = GL_DEBUG_TYPE_OTHER);
  typedef __MessageType MessageType;

  BETTER_ENUM(__MessageSeverity, GLenum, High = GL_DEBUG_SEVERITY_HIGH,
              Medium = GL_DEBUG_SEVERITY_MEDIUM, Low = GL_DEBUG_SEVERITY_LOW,
              Notification = GL_DEBUG_SEVERITY_NOTIFICATION);
  typedef __MessageSeverity MessageSeverity;

  BETTER_ENUM(__BufferBit, GLbitfield, Color = GL_COLOR_BUFFER_BIT, Depth = GL_DEPTH_BUFFER_BIT,
              Stencil = GL_STENCIL_BUFFER_BIT);
  typedef __BufferBit BufferBit;

  BETTER_ENUM(__FramebufferTarget, GLenum, ReadWrite = GL_FRAMEBUFFER, Read = GL_READ_FRAMEBUFFER,
              Write = GL_DRAW_FRAMEBUFFER);
  typedef __FramebufferTarget FramebufferTarget;

  BETTER_ENUM(__Face, GLenum, Front = GL_FRONT, Back = GL_BACK);
  typedef __Face Face;

  BETTER_ENUM(__TextureWrapping, GLenum, Repeat = GL_REPEAT, MirrorRepeat = GL_MIRRORED_REPEAT,
              ClampToEdge = GL_CLAMP_TO_EDGE, ClampToBorder = GL_CLAMP_TO_BORDER);
  typedef __TextureWrapping TextureWrapping;

  BETTER_ENUM(__UniformBlock, GLint, TransformData = 0, CameraData = 1, LightData = 2,
              ShadowData = 3, JointData = 4);
  typedef __UniformBlock UniformBlock;

}  // namespace Engine::GL