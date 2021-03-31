#version 430

in vec3 v_pos;
in vec2 v_uv;
in float time;
out vec4 out_color;

uniform sampler2D u_mainTexture;
uniform vec3 u_color;// = vec3(0.0f, 0.0f, 0.0f);
uniform vec3 v_color;// = vec3(0.0f, 0.0f, 0.0f);
uniform float rate;

vec3 remap(vec3 val) {
    return val * 2.0f + 1.0f;
}

void main() {

  vec3 color = mix(u_color, v_color, rate);
  vec4 texel = texture(u_mainTexture, v_uv).rgba;
  out_color = vec4(texel.rgb + color, texel.a);
  // out_color = vec4(remap(v_pos), 1.0f);
}