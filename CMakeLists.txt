cmake_minimum_required(VERSION 4.0)
project(IPD_main)
set(CMAKE_CXX_STANDARD 20)

add_compile_definitions(OUTPUT_DIR="${PROJECT_SOURCE_DIR}")

set(SOURCE_FILES main.cpp)

add_subdirectory(IPD_lib)
add_executable(IPD_main ${SOURCE_FILES})

target_link_libraries(IPD_main PRIVATE IPD_library)

set(EPS_VALUES 0.00 0.01 0.02 0.03 0.04 0.05 0.06 0.07 0.08 0.09 0.1 0.11 0.12 0.13 0.14 0.15 0.16 0.17 0.18 0.19 0.2)

add_custom_target(noise_sweep
        DEPENDS IPD_main
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMAND ${CMAKE_COMMAND} -E echo "Starting noise sweep..."
        COMMAND ${CMAKE_COMMAND} -E echo ""
)

foreach(eps ${EPS_VALUES})
    add_custom_command(TARGET noise_sweep POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E echo "Running epsilon=${eps}..."
            COMMAND $<TARGET_FILE:IPD_main>
            --rounds 150
            --repeats 30
            --epsilon ${eps}
            --seed 42
            --format csv
            --strategies TFT,GRIM,PAVLOV,CONTRITE
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Running noise sweep for epsilon=${eps}"
    )
endforeach()