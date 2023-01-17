cmake_minimum_required(VERSION 3.23)

project(
    event_loop
    VERSION 0.1.0
    DESCRIPTION "Event loop library in c"
    LANGUAGES C)

link_directories(${CMAKE_BINARY_DIR}/linux/lib64)
include_directories(include)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "-Wall")
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/linux/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/linux/lib64)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/linux/lib64)

file(COPY include DESTINATION ${CMAKE_BINARY_DIR}/linux/include)

add_subdirectory(config)
add_subdirectory(src)
