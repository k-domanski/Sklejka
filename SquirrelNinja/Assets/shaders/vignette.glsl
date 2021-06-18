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
  vec2 ndc      = uv2ndc(fs_in.uv);
  float radius2 = dot(ndc, ndc);
  float coeff   = radius2 / 2.0f;
  float x       = (coeff * coeff - 1.0f);
  float x2      = x * x;
  float base    = -(x2 * x2) + 1.0f;

  vec3 texel_color = texture(u_MainTexture, fs_in.uv).rgb;
  vec3 mixed_color = mix(texel_color, vec3(0.0f), base * u_Factor);

  frag_color = vec4(mixed_color, 1.0f);
}
#endshader