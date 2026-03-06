cmake_minimum_required(VERSION 3.16)
project(mini_grep C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

add_library(minigrep_core
  src/args.c
  src/error.c
  src/grep.c
  src/logger.c
  src/matcher.c
)

target_include_directories(minigrep_core PUBLIC include)

target_compile_options(minigrep_core PRIVATE
  -Wall
  -Wextra
  -Wpedantic
  -Wconversion
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  target_compile_options(minigrep_core PRIVATE -O0 -g3)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
  target_compile_options(minigrep_core PRIVATE -O2 -DNDEBUG)
endif()

add_executable(mini_grep src/main.c)
target_link_libraries(mini_grep PRIVATE minigrep_core)

file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/logs)

enable_testing()
add_test(NAME mini_grep_help
  COMMAND mini_grep
)
set_tests_properties(mini_grep_help PROPERTIES WILL_FAIL TRUE)
