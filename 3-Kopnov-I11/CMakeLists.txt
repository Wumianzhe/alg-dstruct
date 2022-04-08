cmake_minimum_required(VERSION 3.12)
project(lab)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fsanitize=address")

add_executable(main main.c)
include_directories("${PROJECT_SOURCE_DIR}/inc")
add_subdirectory(src)
target_link_libraries(main btree)
