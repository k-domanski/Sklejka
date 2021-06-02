#version 430

#shader vertex
#include "vs_common.incl"

uniform mat4 model;
uniform mat4 projection;
out vec2 v_uv;

uniform vec2 u_Tiling = vec2(1.0f, 1.0f);
uniform vec2 u_Offset = vec2(0.0f, 0.0f);

void main() {
  v_uv        = (a_UV * u_Tiling) + u_Offset;
  gl_Position = projection * model * vec4(a_Position, 1.0f);
}
#endshader

#shader fragment
#include "fs_common.incl"

in vec2 v_uv;
out vec4 out_color;

void main() {
  vec4 texel = texture(u_MainTexture, v_uv);
  out_color  = texel * u_Color;
}
#endshader