cmake_minimum_required(VERSION 4.0)
project(IPD_library)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(src)
add_subdirectory(tests)

include(FetchContent)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz)
FetchContent_MakeAvailable(json)

target_link_libraries(IPD_library PRIVATE nlohmann_json::nlohmann_json)