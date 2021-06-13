#version 430

#shader vertex
#include "vs_common.incl"

void main() {
  gl_Position = u_ModelViewProjection * vec4(a_Position, 1.0f);
}

#endshader

#shader fragment

out vec4 out_color;

uniform float u_Ratio    = 1.0f;
uniform vec4 u_ColorNear = vec4(0.0f, 1.0f, 0.0f, 1.0f);
uniform vec4 u_ColorFar  = vec4(1.0f, 0.0f, 0.0f, 1.0f);
uniform vec4 u_ColorLock = vec4(0.0f, 0.0f, 1.0f, 1.0f);

void main() {
  if (u_Ratio > 0.0f) {
    out_color = mix(u_ColorNear, u_ColorFar, u_Ratio);
  } else {
    out_color = u_ColorLock;
  }
}
#endshader