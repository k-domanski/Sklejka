#version 430 //version before any other shader tag

// sub shaders between #shader and #endshader tags
// after #shader specify shader type: vertex, fragment, geometry, etc
#shader vertex
layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_uv;

void main()
{
    gl_Position = vec4(a_position, 1.0f);
}
#endshader

// detect for missing #endshader tags if new #shader tag appears before previous ends

#shader fragment
// Auto insert #version from the beginning of the section
out vec4 out_color;

void main()
{
    out_color = vec4(1.0f);
}
#endshader