cmake_minimum_required(VERSION 3.10)
project(debug_release_out_of_source C)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(app src/main.c src/math_utils.c)
target_include_directories(app PRIVATE include)
target_compile_options(app PRIVATE -Wall -Wextra)
