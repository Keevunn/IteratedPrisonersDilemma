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

# Collect all test files
set(TEST_SOURCES
    Engine_ArgTests.cpp
    Strategy_BehaviourTests.cpp
    Match_MechanicsTests.cpp
    Math_UtilityTests.cpp
    Tournament_LogicTests.cpp
    Evolution_DynamicsTests.cpp
    Noise_ResistanceTests.cpp
)

# Create test executable
add_executable(Tests_run ${TEST_SOURCES})

# Link against project library and gtest
target_link_libraries(Tests_run IPD_library)
target_link_libraries(Tests_run gtest gtest_main)

# Register tests with CTest
add_test(NAME EngineArguments_Tests COMMAND Tests_run --gtest_filter=EngineArgs*)
add_test(NAME StrategyBehavior_Tests COMMAND Tests_run --gtest_filter=StrategyTest*)
add_test(NAME MatchMechanics_Tests COMMAND Tests_run --gtest_filter=MatchTest*)
add_test(NAME MathUtility_Tests COMMAND Tests_run --gtest_filter=MathTest*)
add_test(NAME TournamentLogic_Tests COMMAND Tests_run --gtest_filter=TournamentTest*)
add_test(NAME EvolutionDynamics_Tests COMMAND Tests_run --gtest_filter=EvolutionTest*)
add_test(NAME NoiseResistance_Tests COMMAND Tests_run --gtest_filter=NoiseTest*)

# Or run all tests together
add_test(NAME AllTests COMMAND Tests_run)