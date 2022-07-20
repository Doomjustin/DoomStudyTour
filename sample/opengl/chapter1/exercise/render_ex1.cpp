#include <cstdlib>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <fmt/format.h>

#include "../shader.h"


void init()
{
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

constexpr unsigned Width = 800;
constexpr unsigned Height = 600;
constexpr unsigned LogSize = 512;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void process_input(GLFWwindow* window);

int main()
{
  init();

  GLFWwindow* window = glfwCreateWindow(Width, Height, "Render Exercise1", nullptr, nullptr);
  if (window == nullptr) {
    fmt::print("Failed to create GLFW window\n");
    glfwTerminate();
    return EXIT_FAILURE;
  }

  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
    fmt::print("Failed to initialize GLAD\n");
    return EXIT_FAILURE;
  }

  // 定义 shader
  auto vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  // 编译
  glShaderSource(vertex_shader, 1, &vertext_shader_source, nullptr);
  glCompileShader(vertex_shader);
  int success;
  char info_log[LogSize];
  glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertex_shader, LogSize, nullptr, info_log);
    fmt::print("ERROR:SHADER::VERTEX::COMPILEATION_FAILED\n{}\n", info_log);
  }

  auto fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment_shader, 1, &fragment_shader_source_orange, nullptr);
  glCompileShader(fragment_shader);
  glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragment_shader, LogSize, nullptr, info_log);
    fmt::print("ERROR:SHADER::FRAGMENT_VERTEX::COMPILEATION_FAILED\n{}\n", info_log);
  }

  auto shader_program = glCreateProgram();
  glAttachShader(shader_program, vertex_shader);
  glAttachShader(shader_program, fragment_shader);
  glLinkProgram(shader_program);
  glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shader_program, LogSize, nullptr, info_log);
    fmt::print("ERROR:SHADER::PROGRAM::LINK_FAILED\n{}\n", info_log);
  }

  // 删除shader
  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);

  // 顶点数组
  float vertices[] = {
    // first triangle
    -0.9f, -0.5f, 0.0f,  // left
    -0.0f, -0.5f, 0.0f,  // right
    -0.45f, 0.5f, 0.0f,  // top
    // second triangle
      0.0f, -0.5f, 0.0f,  // left
      0.9f, -0.5f, 0.0f,  // right
      0.45f, 0.5f, 0.0f   // top
  };

  unsigned VAO;
  glGenVertexArrays(1, &VAO);
  unsigned VBO;
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // 设置顶点属性
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
  glEnableVertexAttribArray(0);

  // 解绑VAO
  glBindVertexArray(0);
  // 解绑VBO
  glBindBuffer(GL_ARRAY_BUFFER, 0);


  while (!glfwWindowShouldClose(window)) {
    process_input(window);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shader_program);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteProgram(shader_program);

  glfwTerminate();
  return EXIT_SUCCESS;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
  glViewport(0, 0, width, height);
}

void process_input(GLFWwindow* window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}