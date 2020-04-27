cmake_minimum_required(VERSION 3.1)

project(Example)

add_executable(spectate spectate.cpp)
target_link_libraries(spectate SDL2main SDL2-static)
