cmake_minimum_required(VERSION 3.0.0)
project(circular_buffer)
file(GLOB_RECURSE SRC_FILES ${PROJECT_SOURCE_DIR}/*.c)
add_executable(test ${SRC_FILES})
target_compile_options (test PRIVATE 
        -std=c99
        -Werror
        -Wall
        -Wextra
        -Wpointer-arith
        -Wcast-align
        -Wwrite-strings
        -Wswitch-default
        -Wunreachable-code
        -Winit-self
        -Wmissing-field-initializers
        -Wno-unknown-pragmas
        -Wstrict-prototypes
        -Wundef
        -Wold-style-definition
        -Wno-misleading-indentation
    )