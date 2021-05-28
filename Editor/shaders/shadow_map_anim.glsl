#version 430

#shader vertex
#include "vs_common.incl"
#include "include/shadow_data.incl"
#include "include/bone_data.incl"

void main() {
  mat4 jointTransform = u_Transforms[a_JointIDs[0]] * a_Weights[0];
  jointTransform += u_Transforms[a_JointIDs[1]] * a_Weights[1];
  jointTransform += u_Transforms[a_JointIDs[2]] * a_Weights[2];
  jointTransform += u_Transforms[a_JointIDs[3]] * a_Weights[3];
  vec4 PosL   = jointTransform * vec4(a_Position, 1.0);
  gl_Position = u_LightSpaceMatrix * u_Model * PosL;
}
#endshader
void main() {
}
#shader fragment
#endshader