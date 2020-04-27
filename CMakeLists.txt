cmake_minimum_required(VERSION 3.1)

project(Brazen)

add_subdirectory(deps/SDL2)

include_directories(PUBLIC include)

add_subdirectory(test)
add_subdirectory(example)
