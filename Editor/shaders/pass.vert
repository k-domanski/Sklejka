#version 430

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_uv;

out vec3 v_pos;
out vec2 v_uv;

uniform mat4 mvp;

void main()
{
    v_pos = a_position;
    v_uv = a_uv;
    gl_Position = mvp * vec4(a_position, 1.0f);
}