cmake_minimum_required(VERSION 3.12)
project(lab2)

set(CMAKE_C_COMPILER "/usr/bin/gcc")
set(CMAKE_CXX_COMPILER "/usr/bin/g++")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_C_FLAGS "-Wall")

include_directories(PUBLIC
  "${PROJECT_SOURCE_DIR}/../lab2")

add_executable(main main.c)
target_link_libraries(main memallocator)
add_subdirectory(src)
add_subdirectory(test)
