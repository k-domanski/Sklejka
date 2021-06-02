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
#include "include/shadow_data.incl"

out vec4 frag_color;
in ShaderData {
  vec2 uv;
  vec3 v_pos;
}
fs_in;
float gamma = 2.2f;
void main() {
  vec3 texel = texture(u_ShadowDepthTexture, fs_in.uv).rrr;
  frag_color = vec4(texel, 1.0f);
}
#endshader