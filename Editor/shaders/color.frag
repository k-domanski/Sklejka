#version 430

in vec3 v_pos;
in vec2 v_uv;
out vec4 out_color;

uniform sampler2D u_mainTexture;
uniform vec3 u_color = vec3(0.0f, 0.0f, 0.0f);

vec3 remap(vec3 val) {
    return val * 2.0f + 1.0f;
}

void main() {
  vec4 texel = texture(u_mainTexture, v_uv).rgba;
  out_color = vec4(texel.rgb + u_color, texel.a);
  // out_color = vec4(remap(v_pos), 1.0f);
}