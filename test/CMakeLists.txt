cmake_minimum_required(VERSION 3.1)

project(Test)

add_executable(async async.cpp)
target_link_libraries(async SDL2main SDL2-static)

add_executable(vector_ops_test vector_ops_test.c)
target_link_libraries(vector_ops_test vector_ops)
