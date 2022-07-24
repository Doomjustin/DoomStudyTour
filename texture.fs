#version 330 core
out vec4 frag_color;

in vec2 texture_coord;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float mix_ratio;

void main()
{
  vec2 flipped_coord = vec2(1.0 - texture_coord.x, texture_coord.y);
  frag_color = mix(texture(texture1, texture_coord), texture(texture2, flipped_coord), mix_ratio);
}