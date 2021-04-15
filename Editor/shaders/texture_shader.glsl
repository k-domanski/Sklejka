#version 430

#shader vertex

#include "common.incl"

uniform mat4 mvp;
uniform mat4 u_model_matrix;

out vec2 v_uv;

void main() {
  v_uv        = a_uv;
  gl_Position = projection_matrix * view_matrix * u_model_matrix * vec4(a_position, 1.0f);
}

#endshader

#shader fragment
uniform sampler2D u_diffuse;

in vec2 v_uv;
out vec4 out_color;

vec3 remap(vec3 val) {
  return val * 2.0f + 1.0f;
}
vec2 remap(vec2 val) {
  return val * 2.0f + 1.0f;
}

void main() {
  vec3 texel = texture(u_diffuse, v_uv).rgb;
  out_color  = vec4(texel, 1.0f);
  //   out_color  = vec4(v_uv, 0.0f, 1.0f);
}
#endshader