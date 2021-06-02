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
      case MessageSeverity::High: {
        LOG_ERROR("[ID: {}][SEVERITY: {}][TYPE: {}][SOURCE: {}]\n{}\n", id,
                  severity_enum._to_string(), type_enum._to_string(), source_enum._to_string(),
                  message);
#if defined(_DEBUG)
        //__debugbreak();
#endif
        break;
      }
      case MessageSeverity::Medium:
      case MessageSeverity::Low: {
        LOG_WARN("[ID: {}][SEVERITY: {}][TYPE: {}][SOURCE: {}]\n{}\n", id,
                 severity_enum._to_string(), type_enum._to_string(), source_enum._to_string(),
                 message);
      }
      default: {
        LOG_INFO("[ID: {}][SEVERITY: {}][TYPE: {}][SOURCE: {}]\n{}\n", id,
                 severity_enum._to_string(), type_enum._to_string(), source_enum._to_string(),
                 message);
        break;
      }
    }
  }
}  // namespace Engine::GL
