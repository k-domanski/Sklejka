#version 430

#shader vertex
#include "vs_common.incl"

out ShaderData {
  vec2 uv;
  vec3 v_pos;
}
vs_out;

void main() {
  vs_out.uv    = a_UV;
  gl_Position  = vec4(a_Position, 1.0f);
  vs_out.v_pos = gl_Position.xyz / gl_Position.w;
}

#endshader

#shader fragment
#include "fs_common.incl"

vec3 GammaCompress(vec3 color, float gamma) {
  return pow(color, vec3(1.0f / gamma));
}

out vec4 frag_color;
in ShaderData {
  vec2 uv;
  vec3 v_pos;
}
fs_in;

uniform float u_Gamma      = 2.2f;
uniform float u_Brightness = 0.0f;
uniform float u_Contrast   = 1.0f;

void main() {
  vec3 texel = texture(u_MainTexture, fs_in.uv).rgb;
  texel      = (texel - 0.5f) * u_Contrast + 0.5f;
  texel      = texel + vec3(u_Brightness);
  texel      = texel / (texel + vec3(1.0f));
  frag_color = vec4(GammaCompress(texel, u_Gamma), 1.0f);
}
#endshader