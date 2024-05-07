cmake_minimum_required(VERSION 3.28)
project(osProject)

set(CMAKE_CXX_STANDARD 20)

add_library(hooks SHARED hooks.cpp)
target_compile_options(hooks PRIVATE -O0 -g -rdynamic)
target_link_libraries(hooks dl)


# Compile matrix_event.cpp into an executable
add_executable(matrix_event matrix_event.cpp common.cpp)
#target_compile_options(matrix_event PRIVATE -O0 -g -finstrument-functions -rdynamic)
#target_link_libraries(matrix_event hooks)

# Compile matrix_thread.cpp into an executable
add_executable(matrix_thread matrix_thread.cpp common.cpp)
#target_compile_options(matrix_thread PRIVATE -O0 -g -finstrument-functions -rdynamic)
#target_link_libraries(matrix_thread hooks)

add_executable(keyvalue_thread keyvalue_thread.cpp common.cpp
        keyvalue.h)
#target_compile_options(keyvalue_thread PRIVATE -O0 -g -finstrument-functions -rdynamic)
#target_link_libraries(keyvalue_thread hooks)

add_executable(keyvalue_event keyvalue_event.cpp common.cpp
        keyvalue.h)

add_executable(file_operations_thread file_operations_thread.cpp common.cpp)

add_executable(file_operations_event file_operations_event.cpp common.cpp)

# Set the LD_PRELOAD environment variable to preload the hooks shared library
set_target_properties(matrix_event PROPERTIES
        ENVIRONMENT "LD_PRELOAD=$<TARGET_FILE:hooks>"
)


