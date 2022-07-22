#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 text_coord;

out vec3 vertex_color;
out vec2 texture_coord;

void main()
{
  gl_Position = vec4(pos, 1.0);
  vertex_color = color;
  texture_coord = text_coord;
}