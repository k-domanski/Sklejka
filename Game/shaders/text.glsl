#version 430

#shader vertex
//#include "vs_common.incl"
layout (location = 0) in vec4 vertex;
uniform mat4 model;
uniform mat4 projection;
out vec2 v_uv;

void main() {
  v_uv = vertex.zw;
  gl_Position = projection * model * vec4(vertex.x, vertex.y, 0.0f, 1.0f);
}
#endshader

#shader fragment
#include "fs_common.incl"

in vec2 v_uv;
out vec4 out_color;

void main() {
  vec4 sampled = vec4(1.0f, 1.0f, 1.0f, texture(u_MainTexture, v_uv).r);
  out_color = vec4(u_MainColor.xyz, 1.0f) * sampled;
}
#endshader