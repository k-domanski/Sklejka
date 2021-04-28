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

layout(std140) uniform u_Directional {
	vec3 u_Direction;

	vec3 u_Ambient;
	vec3 u_Diffuse;
	vec3 u_Specular;
	float u_Intensity;
};

in vec3 v_pos;
in vec2 v_uv;
in vec3 v_normal;
out vec4 out_color;
vec3 calculateDirectionalLight(vec3 normal);
void main() {
	//vec3 viewDirection = normalize(cameraPosition, v_pos);
	vec3 result = calculateDirectionalLight(v_normal);
    out_color = vec4(result, 1.0f);
}

vec3 calculateDirectionalLight(vec3 normal) { //, vec3 viewDirection) {
	/*ambient*/
	vec3 ambient = u_Ambient * texture(u_MainTexture, v_uv).rgb;

	/*diffuse*/
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(-u_Direction);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = u_Diffuse * diff * texture(u_MainTexture, v_uv).rgb;

	///*specular*/
	//vec3 reflectDir = reflect(-lightDir, norm);
	//vec3 halfwayDir = normalize(lightDir + viewDirection);
	//float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
	//vec3 specular = u_Specular * (spec * material.specular);


	vec3 result = (ambient + diffuse /*+ specular*/) * u_Intensity;
	return result;
}
#endshader