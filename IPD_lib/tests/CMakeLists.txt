project(Testing)

include(FetchContent)
FetchContent_Declare(
        googletest
        # Specify the commit you depend on and update it regularly.
        URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Now simply link against gtest or gtest_main as needed. Eg
add_executable(Tests_run Engine_ArgTests.cpp)
target_link_libraries(Tests_run IPD_library)
target_link_libraries(Tests_run gtest gtest_main)
add_test(NAME EngineArguments_Tests COMMAND Tests_run)