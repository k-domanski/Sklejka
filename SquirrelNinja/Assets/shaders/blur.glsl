#version 430

#shader vertex
#include "vs_common.incl"

out ShaderData {
  vec2 uv;
}
vs_out;

void main() {
  vs_out.uv   = a_UV;
  gl_Position = vec4(a_Position, 1.0f);
}
#endshader

#shader fragment
#include "fs_common.incl"
out vec4 frag_color;

in ShaderData {
  vec2 uv;
}
fs_in;

uniform int u_Horizontal = 1;
uniform float weight[5]  = float[](0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

void main() {
  vec2 texel_size = 1.0 / textureSize(u_MainTexture, 0);
  vec4 result     = texture(u_MainTexture, fs_in.uv) * weight[0];
  vec2 offset     = {0.0f, 0.0f};
  if (u_Horizontal == 1) {
    offset = vec2(texel_size.x, 0.0f);
  } else {
    offset = vec2(0.0f, texel_size.y);
  }

  for (int i = 1; i < 5; ++i) {
    result += texture(u_MainTexture, fs_in.uv + i * offset) * weight[i];
    result += texture(u_MainTexture, fs_in.uv - i * offset) * weight[i];
  }

  result.a   = 1.0f;
  frag_color = result;
}
#endshader