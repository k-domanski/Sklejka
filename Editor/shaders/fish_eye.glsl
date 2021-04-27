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

float factor = 0.5f;

void main() {
  vec2 ndc     = uv2ndc(fs_in.uv);
  float theta  = atan(ndc.y, ndc.x);
  float radius = length(ndc);
  radius       = pow(radius, (1 - radius * factor));
  ndc.x        = radius * cos(theta);
  ndc.y        = radius * sin(theta);
  vec2 uv      = ndc2uv(ndc);
  vec3 texel   = texture(u_MainTexture, uv).rgb;
  frag_color   = vec4(texel, 1.0f);
}
#endshader