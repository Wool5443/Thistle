cmake_minimum_required(VERSION 3.30)
project(Thistle C)

# add_compile_definitions("DISABLE_LOGGING")

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED true)

add_subdirectory(cmlib)
add_subdirectory(common)
add_subdirectory(front)
add_subdirectory(back)
