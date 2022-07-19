include(FetchContent)

# FetchContent_MakeAvailable was not added until CMake 3.14
if(${CMAKE_VERSION} VERSION_LESS 3.14)
    include(addFetchContentMakeAvailable.cmake)
endif()

FetchContent_Declare(
  GLFW
  GIT_REPOSITORY    git@github.com:glfw/glfw.git # 指定git仓库地址
  GIT_TAG           3.3-stable # 指定版本
  SOURCE_DIR        ${CMAKE_SOURCE_DIR}/third-party/GLFW   # 源码下载到此目录下
)

FetchContent_MakeAvailable(GLFW)