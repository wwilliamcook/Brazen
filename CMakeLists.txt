cmake_minimum_required(VERSION 3.1)

project(Brazen)

add_subdirectory(deps/SDL2)

add_library(vector_ops src/vector_ops.c include/vector_ops.h)
target_include_directories(vector_ops PRIVATE include)

include_directories(PUBLIC include)

add_subdirectory(test)
add_subdirectory(example)
