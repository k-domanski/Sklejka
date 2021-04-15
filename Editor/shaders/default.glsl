#version 430  // version before any other shader tag

// sub shaders between #shader and #endshader tags
// after #shader specify shader type: vertex, fragment, geometry, etc
#shader vertex
#include "common.incl"

uniform mat4 mvp;
uniform mat4 u_model_matrix;

out vec3 v_pos;
out vec2 v_uv;

void main() {
  v_uv = a_uv;
  // gl_Position = mvp * vec4(a_position, 1.0f);
  gl_Position = projection_matrix * view_matrix * u_model_matrix * vec4(a_position, 1.0f);
  v_pos       = gl_Position.xyz / gl_Position.w;
}
#endshader

// detect for missing #endshader tags if new #shader tag appears before previous ends

#shader fragment
// Auto insert #version from the beginning of the section
in vec3 v_pos;
in vec2 v_uv;
out vec4 out_color;

uniform sampler2D u_mainTexture;

vec3 remap(vec3 val) {
  return val * 2.0f + 1.0f;
}

void main() {
  vec3 texel = texture(u_mainTexture, v_uv).rgb;
  out_color  = vec4(texel, 1.0f);
  // out_color = vec4(remap(v_pos), 1.0f);
}
#endshader