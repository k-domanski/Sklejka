#version 430

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_uv;

out vec3 v_pos;
out vec2 v_uv;

uniform float u_time = 0.0f;

void main()
{
    v_pos = a_position * sin(u_time);
    v_uv = a_uv;
    gl_Position = vec4(v_pos, 1.0f);
}