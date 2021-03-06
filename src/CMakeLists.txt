cmake_minimum_required(VERSION 3.0.0)
project(circular_buffer)
file(GLOB_RECURSE SRC_FILES ${PROJECT_SOURCE_DIR}/*.c)
add_executable(test ${SRC_FILES})