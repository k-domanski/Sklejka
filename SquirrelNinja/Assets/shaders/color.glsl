#version 430

#shader vertex
#include "vs_common.incl"

void main() {
  gl_Position = u_ModelViewProjection * vec4(a_Position, 1.0f);
}
#endshader

#shader fragment
#include "fs_common.incl"

in vec3 v_pos;
in vec2 v_uv;
out vec4 out_color;

void main() {
  out_color = u_Color;
}
#endshader