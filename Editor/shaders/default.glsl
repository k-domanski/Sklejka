#version 430

#shader vertex
#include "vs_common.incl"

uniform mat4 mvp;
uniform mat4 u_model_matrix;

out vec3 v_pos;
out vec2 v_uv;

void main() {
  v_uv = a_uv;
  gl_Position = projection_matrix * view_matrix * u_model_matrix * vec4(a_position, 1.0f);
  v_pos       = gl_Position.xyz / gl_Position.w;
}
#endshader

#shader fragment
#include "fs_common.incl"

in vec3 v_pos;
in vec2 v_uv;
out vec4 out_color;

void main() {
  vec3 texel = texture(u_mainTexture, v_uv).rgb;
  out_color  = vec4(texel, 1.0f);
  // out_color = vec4(remap(v_pos), 1.0f);
}
#endshader