#version 430

#shader vertex
#include "vs_common.incl"
#include "include/shadow_data.incl"

out ShaderData {
  vec4 pos;
  vec2 uv;
  vec3 normal;
  vec4 lightSpacePos;
}
vs_out;
void main() {
  vs_out.uv            = a_UV;
  vs_out.normal        = mat3(transpose(inverse(u_Model))) * a_Normal;
  gl_Position          = u_ModelViewProjection * vec4(a_Position, 1.0f);
  vs_out.pos           = gl_Position;
  vs_out.lightSpacePos = u_LightSpaceMatrix * u_Model * vec4(a_Position, 1.0f);
}
#endshader

#shader fragment
#include "fs_common.incl"
#include "include/fs_light.incl"

in ShaderData {
  vec4 pos;
  vec2 uv;
  vec3 normal;
  vec4 lightSpacePos;
}
fs_in;
out vec4 out_color;

void main() {
  vec3 lightSpacePosUV = ndc2uv(fs_in.lightSpacePos.xyz / fs_in.lightSpacePos.w);
  float shadowMapDepth = texture(u_ShadowDepthTexture, lightSpacePosUV.xy).r;

  vec3 normal  = normalize(fs_in.normal);
  float bias   = u_ShadowBias;
  float shadow = calculateShadow(lightSpacePosUV, u_ShadowDepthTexture, bias);


  // vec3 result = PBR(normal, vec3 V, vec3 L, vec3 F0, vec3 albedo, float roughness, float metalness);
  vec3 result = calculateDirectionalLightColor(fs_in.normal, u_Color, shadow);
  out_color   = vec4(result, 1.0f);
}
#endshader