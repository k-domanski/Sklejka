#version 430

in vec3 v_pos;
out vec4 out_color;

vec3 remap(vec3 val) {
    return val * 2.0f + 1.0f;
}

void main() {
  out_color = vec4(remap(v_pos), 1.0f);
}