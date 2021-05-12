#version 430

#shader vertex
#include "vs_common.incl"

uniform mat4 model;
uniform mat4 projection;
out vec2 v_uv;

void main() {
  v_uv = a_UV;
  gl_Position = projection * model * vec4(a_Position.x, a_Position.y, 0.0f, 1.0f);
}
#endshader

#shader fragment
#include "fs_common.incl"

in vec2 v_uv;
out vec4 out_color;

uniform float ratio;
uniform bool horizontal;

void main() {
  float alpha = 1.0f;
  if(horizontal) {
    if(v_uv.x > ratio)
    alpha = 0.0f;
  }
  else {
      if(v_uv.y > ratio)
      alpha = 0.0f;
  }
  vec4 texel = texture(u_MainTexture, v_uv);
  out_color  = vec4(u_MainColor.xyz, alpha) * texel;
}
#endshader