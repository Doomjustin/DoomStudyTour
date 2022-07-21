find_package(OpenGL REQUIRED)

add_library(LearnOpenGL
  Shader.h
  Shader.cpp
  stb_image.h
  stb_image.cpp
)

link_libraries(OpenGL::GL glad glfw LearnOpenGL)

add_subdirectory(chapter1)
