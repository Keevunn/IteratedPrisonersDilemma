cmake_minimum_required(VERSION 4.0)
project(IPD_main)
set(CMAKE_CXX_STANDARD 20)

add_compile_definitions(OUTPUT_DIR="${PROJECT_SOURCE_DIR}")

set(SOURCE_FILES main.cpp)

add_subdirectory(IPD_lib)
add_executable(IPD_main ${SOURCE_FILES})

target_link_libraries(IPD_main PRIVATE IPD_library)
