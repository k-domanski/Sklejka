#version 430

#shader vertex
#include "vs_common.incl"

// uniform mat4 mvp;
// uniform mat4 u_model_matrix;

out vec3 v_pos;
out vec2 v_uv;
out vec3 v_normal;

void main() {
  v_uv = a_UV;
  v_normal = mat3(transpose(inverse(u_Model))) * a_Normal;
  gl_Position = u_ModelViewProjection * vec4(a_Position, 1.0f);
  v_pos       = gl_Position.xyz / gl_Position.w;
}
#endshader

#shader fragment
#include "fs_common.incl"
#include "include/fs_light.incl"


in vec3 v_pos;
in vec2 v_uv;
in vec3 v_normal;
out vec4 out_color;
void main() {
	/*view direction for specular*/
	//vec3 viewDirection = normalize(cameraPosition, v_pos);
	vec3 result = calculateDirectionalLight(v_normal, v_uv);
    out_color = vec4(result, 1.0f);
}
#endshader