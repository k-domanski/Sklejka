#version 430

#shader vertex
layout(location = 0) vec3 a_Position;
layout(location = 1) vec3 a_Color;
layout(location = 2) vec2 a_Scale;
layout(location = 3) float a_Rotation;

struct Vert2Geo {
  vec3 color;
  vec2 scale;
  float rotation;
};

out Vert2Geo vs_out;

void main() {
  gl_Position     = vec4(a_Position, 1.0f);
  vs_out.color    = a_Color;
  vs_out.scale    = a_Scale;
  vs_out.rotation = a_Rotation;
}

#endshader

#shader geometry
layout(points) in;
layout(triangle_strip) out;
layout(max_vertices = 4) out;

#include "include/vs_data.incl"

struct Vert2Geo {
  vec3 color;
  vec2 scale;
  float rotation;
};

struct Geo2Frag {
  vec3 color;
  vec2 uv;
};

in Vert2Geo geo_in;
out Geo2Frag geo_out;

void main() {
  /* Create quad */
  mat4 model      = mat4(1.0f);
  model[0][0]     = geo_in.scale.x;
  model[1][1]     = geo_in.scale.y;
  vec4 position   = gl_in[0].gl_Position;
  vec4 camera_pos = vec4(-(transpose(mat3(u_View)) * u_View[3].xyz), 0.0f);
  model[3]        = position - camera_pos;
  vec4 offset;

  /* Bottom Left */
  offset        = vec4(-0.5f, -0.5f, 0.0f, 1.0f);
  gl_Position   = u_Projection * model * offset;
  geo_out.color = geo_in.color;
  geo_out.uv    = vec2(0.0f, 0.0f);
  EmitVertex();

  /* Bottom Right */
  offset        = vec4(0.5f, -0.5f, 0.0f, 0.0f);
  gl_Position   = u_Projection * model * offset;
  geo_out.color = geo_in.color;
  geo_out.uv    = vec2(1.0f, 0.0f);
  EmitVertex();

  /* Top Left */
  offset        = vec4(-0.5f, 0.5f, 0.0f, 1.0f);
  gl_Position   = u_Projection * model * offset;
  geo_out.color = geo_in.color;
  geo_out.uv    = vec2(0.0f, 1.0f);
  EmitVertex();

  /* Top Right */
  offset        = vec4(0.5f, 0.5f, 0.0f, 1.0f);
  gl_Position   = u_Projection * model * offset;
  geo_out.color = geo_in.color;
  geo_out.uv    = vec2(1.0f, 1.0f);
  EmitVertex();
}

#endshader

#shader fragment

struct Geo2Frag {
  vec3 color;
  vec2 uv;
};

in Geo2Frag fs_in;
out vec4 final_color;

void main() {
  vec4 texel_color = texture2D(u_MainTexture, fs_in.uv);
  final_color      = texel_color * vec4(fs_in.color, 1.0f);
}

#endshader