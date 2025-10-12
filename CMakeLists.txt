cmake_minimum_required(VERSION 4.0)
project(CSC8501_Assignment)
set(CMAKE_CXX_STANDARD 20)

set(SOURCE_FILES main.cpp)

add_subdirectory(IPD_lib)
add_executable(CSC8501_Assignment ${SOURCE_FILES})

target_link_libraries(CSC8501_Assignment PRIVATE IPD_library)

add_subdirectory(Google_tests)