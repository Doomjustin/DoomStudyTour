#ifndef DOOM_STUDY_TOUR_OPENGL_SHADER_H
#define DOOM_STUDY_TOUR_OPENGL_SHADER_H


/* 典型GLSL写的shader程序

  #version version_number
  in type in_variable_name;
  in type in_variable_name;

  out type out_variable_name;

  uniform type uniform_name;

  int main()
  {
    // 处理输入并进行一些图形操作
    ...
    // 输出处理过的结果到输出变量
    out_variable_name = weird_stuff_we_processed;
  }
  
 */

constexpr const char* vertext_shader_source =
R"(#version 330 core
layout (location = 0) in vec3 pos;

void main()
{
  gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);
}
)";

constexpr const char* fragment_shader_source_orange =
R"(#version 330 core
out vec4 frag_color;

void main()
{
  frag_color = vec4(1.0f, 0.5f, 0.2f, 1.0f);
}
)";

constexpr const char* fragment_shader_source_yellow =
R"(#version 330 core
out vec4 frag_color;

void main()
{
  frag_color = vec4(1.0f, 1.0f, 0.0f, 1.0f);
}
)";


#endif // DOOM_STUDY_TOUR_OPENGL_SHADER_H