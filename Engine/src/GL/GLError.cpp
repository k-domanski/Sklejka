#include "pch.h"
#include "GLError.h"
#include <App/Log.h>
#include <GL/GLCore.h>

namespace Engine::GL {
  auto GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
                                  GLsizei length, const GLchar* message, const void* userParam)
      -> void {
    auto source_enum   = MessageSource::_from_integral_unchecked(source);
    auto type_enum     = MessageType::_from_integral_unchecked(type);
    auto severity_enum = MessageSeverity::_from_integral_unchecked(severity);
    switch (severity_enum) {
      case MessageSeverity::High:
      case MessageSeverity::Medium:
      case MessageSeverity::Low: {
        CORE_ERROR("[SEVERITY: {0}][TYPE: {1}][SOURCE: {2}]\n{3}\n", severity_enum._to_string(),
                   type_enum._to_string(), source_enum._to_string(), message);
        break;
      }
      default: {
        CORE_INFO("[SEVERITY: {0}][TYPE: {1}][SOURCE: {2}]\n{3}\n", severity_enum._to_string(),
                  type_enum._to_string(), source_enum._to_string(), message);
        break;
      }
    }
  }
}  // namespace Engine::GL
