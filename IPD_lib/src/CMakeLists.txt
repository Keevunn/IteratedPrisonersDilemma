file(GLOB HEADER_LIST
        CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/include/*.h")
add_library(IPD_library Engine.cpp ${HEADER_LIST})

target_include_directories(IPD_library PUBLIC include)
target_compile_features(IPD_library PUBLIC cxx_std_20)

source_group(
        TREE "${PROJECT_SOURCE_DIR}/include"
        PREFIX "Header Files"
        FILES ${HEADER_LIST}
)