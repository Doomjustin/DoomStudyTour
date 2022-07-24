find_package(OpenGL REQUIRED)

add_library(LearnOpenGL
  Shader.h
  Shader.cpp
  stb_image.h
  stb_image.cpp
  Camera.h
  Camera.cpp
)

target_link_libraries(LearnOpenGL PUBLIC OpenGL::GL glfw glad glm)

link_libraries(LearnOpenGL)

add_subdirectory(chapter1)
