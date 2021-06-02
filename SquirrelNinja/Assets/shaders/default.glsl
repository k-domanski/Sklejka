#version 430

#shader vertex
#include "vs_common.incl"

// uniform mat4 mvp;
// uniform mat4 u_model_matrix;

out vec3 v_pos;
out vec2 v_uv;

void main() {
  v_uv = a_UV;
  gl_Position = u_ModelViewProjection * vec4(a_Position, 1.0f);
  v_pos       = gl_Position.xyz / gl_Position.w;
}
#endshader

#shader fragment
#include "fs_common.incl"

in vec3 v_pos;
in vec2 v_uv;
out vec4 out_color;

void main() {
  vec3 texel = texture(u_MainTexture, v_uv).rgb;
  out_color  = vec4(texel, 1.0f);
}
#endshader