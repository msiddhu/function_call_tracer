cmake_minimum_required(VERSION 3.28)
project(osProject)

set(CMAKE_CXX_STANDARD 20)

add_executable(osProject main.cpp
        matrix_thread.h
        matrix_thread.cpp
        matrix_event.cpp
        matrix_event.h
        KeyValueStore.cpp
        KeyValueStore.h)

