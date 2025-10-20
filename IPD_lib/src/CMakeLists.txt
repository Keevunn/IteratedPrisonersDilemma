file(GLOB HEADER_LIST
        CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/include/*.h" "${PROJECT_SOURCE_DIR}/include/StrategyAgents/*.h" "${PROJECT_SOURCE_DIR}/include/Output/*.h")
file(GLOB SOURCE_LIST
        CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/src/*.cpp" "${PROJECT_SOURCE_DIR}/src/StrategyAgents/*.cpp" "${PROJECT_SOURCE_DIR}/src/Output/*.cpp")
add_library(IPD_library ${HEADER_LIST} ${SOURCE_LIST})

target_include_directories(IPD_library PUBLIC include)
target_compile_features(IPD_library PUBLIC cxx_std_20)

source_group(
        TREE "${PROJECT_SOURCE_DIR}/include"
        PREFIX "Header Files"
        FILES ${HEADER_LIST}
)