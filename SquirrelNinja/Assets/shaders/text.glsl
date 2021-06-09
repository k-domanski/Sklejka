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
  vec4 texel = gamma2linear(vec4(1.0f, 1.0f, 1.0f, texture(u_MainTexture, v_uv).r));
  texel = (texel - 0.5f) * u_Contrast + 0.5f;
  texel = texel + vec4(u_Brightness, u_Brightness, u_Brightness, 0.0f);
  vec4 res = vec4(u_Color.xyz, 1.0f) * texel;
  out_color = vec4(GammaCompress(res.rgb, u_Gamma), res.a);
}
#endshader