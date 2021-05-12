#version 430

#shader vertex
#include "vs_common.incl"
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
  gl_Position = u_ViewProjection * vs_out.posWS;
  /* Retreive camera pos */
  vec3 col4        = (u_View * vec4(0.0f, 0.0f, 0.0f, 1.0f)).xyz;
  vs_out.cameraPos = -(transpose(mat3(u_View)) * col4);
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
  vec3 texel_color     = gamma2linear(texture(u_MainTexture, fs_in.uv)).rgb * u_Color.rgb;

  vec3 normal    = normalize(fs_in.normal);
  vec3 view_dir  = normalize(fs_in.cameraPos - fs_in.posWS.xyz);
  vec3 light_dir = normalize(-u_LightDirection);

  vec3 F0 = mix(vec3(u_F0), texel_color, u_Metalness);

  float bias   = u_ShadowBias;
  float shadow = calculateShadow(lightSpacePosUV, u_ShadowDepthTexture, bias);

  vec3 pbr = u_Diffuse * u_Intensity
             * PBR(normal, view_dir, light_dir, F0, texel_color, u_Roughness, u_Metalness);
  vec3 result = pbr * (1.0f - shadow) + u_Ambient * texel_color;
  out_color   = vec4(result, 1.0f);
}
#endshader