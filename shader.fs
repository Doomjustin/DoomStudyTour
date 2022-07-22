#version 330 core
out vec4 frag_color;

in vec3 vertex_color;
in vec3 vertex_pos;

void main()
{
  // frag_color = vec4(vertex_color, 1.0f);
  frag_color = vec4(vertex_pos, 1.0f);
}