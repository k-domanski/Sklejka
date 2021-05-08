#version 430

#shader vertex
#include "vs_common.incl"
#include "include/shadow_data.incl"

out ShaderData {
  vec3 pos;
  vec2 uv;
  vec3 normal;
  vec4 lightSpacePos;
}
vs_out;

void main() {
  vs_out.uv            = a_UV;
  vs_out.normal        = mat3(transpose(inverse(u_Model))) * a_Normal;
  gl_Position          = u_ModelViewProjection * vec4(a_Position, 1.0f);
  vs_out.pos           = gl_Position.xyz / gl_Position.w;
  vs_out.lightSpacePos = u_LightSpaceMatrix * u_Model * vec4(a_Position, 1.0f);
}
#endshader

#shader fragment
#include "fs_common.incl"
#include "include/fs_light.incl"

in ShaderData {
  vec3 pos;
  vec2 uv;
  vec3 normal;
  vec4 lightSpacePos;
}
fs_in;
out vec4 out_color;
void main() {
  /*view direction for specular*/
  // vec3 viewDirection = normalize(cameraPosition, v_pos);
  vec3 lightSpacePosUV = ndc2uv(fs_in.lightSpacePos.xyz / fs_in.lightSpacePos.w);
  float shadowMapDepth = texture(u_ShadowDepthTexture, lightSpacePosUV.xy).r;

  vec3 normal  = normalize(fs_in.normal);
  // float bias   = max(0.05 * (1.0 - dot(normal, -u_Direction)), u_ShadowBias);
  float bias   = u_ShadowBias;
  float shadow = calculateShadow(lightSpacePosUV, shadowMapDepth, bias);

  vec3 result = calculateDirectionalLight(fs_in.normal, fs_in.uv, shadow);
  out_color   = vec4(result, 1.0f);
}
#endshader