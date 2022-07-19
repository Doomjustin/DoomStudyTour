find_package(OpenGL REQUIRED)

link_libraries(OpenGL::GL glad glfw)


add_executable(first first.cpp)

add_executable(render render.cpp)