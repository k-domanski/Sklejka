#version 430

in vec3 v_pos;
in vec2 v_uv;
out vec4 out_color;

uniform sampler2D u_mainTexture;

vec3 remap(vec3 val) {
    return val * 2.0f + 1.0f;
}

void main() {
  vec3 texel = texture(u_mainTexture, v_uv).rgb;
  out_color = vec4(texel, 1.0f);
  // out_color = vec4(remap(v_pos), 1.0f);
}