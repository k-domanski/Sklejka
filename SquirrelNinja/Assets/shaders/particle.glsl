#version 430

#shader vertex
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_Scale;

out Vert2Geo {
  vec2 scale;
}
vs_out;

void main() {
  gl_Position     = vec4(a_Position, 1.0f);
  vs_out.scale    = a_Scale;
}

#endshader

#shader geometry
layout(points) in;
layout(triangle_strip) out;
layout(max_vertices = 4) out;

#include "include/vs_data.incl"

in Vert2Geo {
  vec2 scale;
}
geo_in[];

out Geo2Frag {
  vec2 uv;
}
geo_out;

void main() {
  /* Create quad */
  mat4 model = mat4(1.0f);
  model[3]   = gl_in[0].gl_Position;

  mat4 model_view  = u_View * model;
  model_view[0][0] = 1.0f;
  model_view[0][1] = 0.0f;
  model_view[0][2] = 0.0f;

  model_view[1][0] = 0.0f;
  model_view[1][1] = 1.0f;
  model_view[1][2] = 0.0f;

  model_view[2][0] = 0.0f;
  model_view[2][1] = 0.0f;
  model_view[2][2] = 1.0f;

  vec4 offset;
  vec2 scale = geo_in[0].scale;
  /* Bottom Left */
  offset        = vec4(-0.5f * scale.x, -0.5f * scale.y, 0.0f, 1.0f);
  gl_Position   = u_Projection * model_view * offset;
  geo_out.uv    = vec2(0.0f, 0.0f);
  EmitVertex();

  /* Bottom Right */
  offset        = vec4(0.5f * scale.x, -0.5f * scale.y, 0.0f, 1.0f);
  gl_Position   = u_Projection * model_view * offset;
  geo_out.uv    = vec2(1.0f, 0.0f);
  EmitVertex();

  /* Top Left */
  offset        = vec4(-0.5f * scale.x, 0.5f * scale.y, 0.0f, 1.0f);
  gl_Position   = u_Projection * model_view * offset;
  geo_out.uv    = vec2(0.0f, 1.0f);
  EmitVertex();

  /* Top Right */
  offset        = vec4(0.5f * scale.x, 0.5f * scale.y, 0.0f, 1.0f);
  gl_Position   = u_Projection * model_view * offset;
  geo_out.uv    = vec2(1.0f, 1.0f);
  EmitVertex();
}

#endshader

#shader fragment

#include "fs_common.incl"

in Geo2Frag {
  vec2 uv;
}
fs_in;

out vec4 final_color;

void main() {
  vec4 texel_color = texture2D(u_MainTexture, fs_in.uv);
  final_color      = texel_color * u_Color;
}

#endshader