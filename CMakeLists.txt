cmake_minimum_required(VERSION 3.28)
project(osProject)

set(CMAKE_CXX_STANDARD 20)

# Add compile options
#add_compile_options(-O0 -g -finstrument-functions -rdynamic)

add_executable(matrix matrix.cpp
        matrix_thread.h
        matrix_thread.cpp
        matrix_event.cpp
        matrix_event.h
)

