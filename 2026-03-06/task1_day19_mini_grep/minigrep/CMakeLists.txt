cmake_minimum_required(VERSION 3.16)
project(mini_grep C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(mini_grep
    src/main.c
    src/args.c
    src/grep.c
)
target_include_directories(mini_grep PUBLIC include)
