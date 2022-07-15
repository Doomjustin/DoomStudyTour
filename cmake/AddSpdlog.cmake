include(FetchContent)

# FetchContent_MakeAvailable was not added until CMake 3.14
if(${CMAKE_VERSION} VERSION_LESS 3.14)
    include(addFetchContentMakeAvailable.cmake)
endif()

FetchContent_Declare(
  spdlog
  GIT_REPOSITORY    https://github.com/gabime/spdlog.git # 指定git仓库地址
  GIT_TAG           v1.10.0 # 指定版本
  SOURCE_DIR        ${CMAKE_SOURCE_DIR}/third-party/spdlog # 源码下载到此目录下
)

FetchContent_MakeAvailable(spdlog)