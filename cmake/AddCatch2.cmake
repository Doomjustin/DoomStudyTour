include(FetchContent)

# FetchContent_MakeAvailable was not added until CMake 3.14
if(${CMAKE_VERSION} VERSION_LESS 3.14)
    include(addFetchContentMakeAvailable.cmake)
endif()

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY    https://github.com/catchorg/Catch2.git # 指定git仓库地址
  GIT_TAG           v3.0.1 # 指定版本
  SOURCE_DIR        ${CMAKE_SOURCE_DIR}/third-party/Catch2   # 源码下载到此目录下
)

FetchContent_MakeAvailable(Catch2)