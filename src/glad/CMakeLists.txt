add_library(glad
  STATIC
    ${PROJECT_SOURCE_DIR}/include/glad/glad.h
    ${PROJECT_SOURCE_DIR}/include/KHR/khrplatform.h
    glad.c
)