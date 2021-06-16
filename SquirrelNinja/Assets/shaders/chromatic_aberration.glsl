#version 430

#shader vertex
#include "include/vs_attributes.incl"

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

vec2 uv2ndc(vec2 uv) {
  return uv * 2 - 1.0f;
}
vec2 ndc2uv(vec2 ndc) {
  return ndc * 0.5f + 0.5f;
}
out vec4 frag_color;
in ShaderData {
  vec2 uv;
}
fs_in;

uniform float u_Factor = 0.0f;

void main() {
  vec2 texel_size = 1.0 / textureSize(u_MainTexture, 0);

  vec2 ndc      = uv2ndc(fs_in.uv);
  float radius2 = dot(ndc, ndc);
  float coeff   = max(0.5f, radius2 / 2.0f);
  vec2 x        = (ndc * ndc - vec2(1.0f));
  vec2 x2       = x * x;
  vec2 base     = -(x2 * x2) + vec2(1.0f);
  vec2 offset   = base * coeff * u_Factor * texel_size;

  vec3 aberration_color = vec3(0.0f);
  aberration_color.r    = texture(u_MainTexture, fs_in.uv + offset * 1.0f).r;
  aberration_color.g    = texture(u_MainTexture, fs_in.uv + offset * -2.0f).g;
  aberration_color.b    = texture(u_MainTexture, fs_in.uv + offset * 3.0f).b;

  frag_color = vec4(aberration_color, 1.0f);
}
#endshader