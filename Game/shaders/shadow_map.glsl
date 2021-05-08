#version 430

#shader vertex
#include "vs_common.incl"
#include "include/shadow_data.incl"

void main() {
  gl_Position = u_LightSpaceMatrix * u_Model * vec4(a_Position, 1.0f);
}
#endshader
void main() {
}
#shader fragment
#endshader