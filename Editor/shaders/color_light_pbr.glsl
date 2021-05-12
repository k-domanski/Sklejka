#version 430

#shader vertex
#include "vs_common.incl"
#include "include/light_data.incl"
#include "include/shadow_data.incl"

out ShaderData {
  vec4 posWS;
  vec3 normal;
  vec2 uv;
  vec4 lightSpacePos;
  vec3 cameraPos;
}
vs_out;
void main() {
  vs_out.uv            = a_UV;
  vs_out.normal        = mat3(transpose(inverse(u_Model))) * a_Normal;
  vs_out.posWS         = u_Model * vec4(a_Position, 1.0f);
  vs_out.lightSpacePos = u_LightSpaceMatrix * u_Model * vec4(a_Position, 1.0f);
  vs_out.cameraPos     = (u_View * vec4(0.0f, 0.0f, 0.0f, -1.0f)).xyz;
  gl_Position          = u_ViewProjection * vs_out.posWS;
}
#endshader

#shader fragment
#include "fs_common.incl"
#include "include/fs_light.incl"

in ShaderData {
  vec4 posWS;
  vec3 normal;
  vec2 uv;
  vec4 lightSpacePos;
  vec3 cameraPos;
}
fs_in;
out vec4 out_color;

void main() {
  vec3 lightSpacePosUV = ndc2uv(fs_in.lightSpacePos.xyz / fs_in.lightSpacePos.w);

  vec3 normal    = normalize(fs_in.normal);
  vec3 view_dir  = normalize(fs_in.cameraPos - fs_in.posWS.xyz);
  vec3 light_dir = normalize(-u_LightDirection);

  vec3 F0 = mix(vec3(u_F0), u_Color.rgb, u_Metalness);

  float bias   = u_ShadowBias;
  float shadow = calculateShadow(lightSpacePosUV, u_ShadowDepthTexture, bias);

  vec3 pbr = u_Diffuse * u_Intensity
             * PBR(normal, view_dir, light_dir, F0, u_Color.rgb, u_Roughness, u_Metalness);
  vec3 result = pbr * (1.0f - shadow) + u_Ambient * u_Color.rgb;
  out_color   = vec4(result, 1.0f);
}
#endshader