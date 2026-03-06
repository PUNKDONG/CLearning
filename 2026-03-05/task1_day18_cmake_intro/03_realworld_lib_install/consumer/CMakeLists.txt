cmake_minimum_required(VERSION 3.16)
project(netfmt_consumer C)

find_package(netfmt CONFIG REQUIRED)

add_executable(consumer src/main.c)
target_link_libraries(consumer PRIVATE netfmt::netfmt_static)
