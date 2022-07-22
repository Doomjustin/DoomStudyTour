#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 color;

out vec3 vertex_color;
out vec3 vertex_pos;

uniform float offset;

void main()
{
  gl_Position = vec4(pos.x + offset, pos.y, pos.z, 1.0f);
  vertex_color = color;
  vertex_pos = pos;
}