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
out vec4 frag_color;

in ShaderData {
  vec2 uv;
  vec3 v_pos;
}
fs_in;

void main() {
  // frag_color = vec4(vec3(1.0f) - texture(u_MainTexture, fs_in.uv).rgb, 1.0f);
  frag_color = vec4(texture(u_MainTexture, fs_in.uv).rgb * remap(fs_in.v_pos), 1.0f);
}
#endshader