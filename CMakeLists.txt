cmake_minimum_required(VERSION 4.0)
project(Main)
set(CMAKE_CXX_STANDARD 20)

set(SOURCE_FILES main.cpp)

add_subdirectory(IPD_lib)
add_executable(Main ${SOURCE_FILES})

target_link_libraries(Main PRIVATE IPD_library)
