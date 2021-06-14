#version 430

#shader vertex
#include "vs_common.incl"

out vec3 direction;

void main() {
  direction   = a_Position;
  vec4 pos    = u_Projection * mat4(mat3(u_View)) * vec4(a_Position, 1.0);
  gl_Position = pos.xyww;
}
#endshader

#shader fragment
#include "fs_common.incl"

in vec3 direction;
out vec4 out_color;

uniform samplerCube u_Skybox;

void main() {
  vec4 texel = gamma2linear(texture(u_Skybox, direction));
  texel = (texel - 0.5f) * u_Contrast + 0.5f;
  texel = texel + vec4(u_Brightness, u_Brightness, u_Brightness, 0.0f);
  out_color = vec4(texel.rgb, texel.a);
}
#endshader