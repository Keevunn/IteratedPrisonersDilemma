#include <fstream>
#include <gtest/gtest.h>
#include "../include/Engine.h"

// Helper to build argc/argv from vector<string>
static void runArgs(const std::vector<std::string_view>& args) {
    Engine::Engine e(args);
}

// --------------------------- VALID CASES ---------------------------

TEST(EngineArgs, RoundsValid) {
    EXPECT_NO_THROW(runArgs({"--rounds", "200"}));
}

TEST(EngineArgs, RepeatsValid) {
    EXPECT_NO_THROW(runArgs({"--repeats", "5"}));
}

TEST(EngineArgs, SeedValid) {
    EXPECT_NO_THROW(runArgs({"--seed", "123"}));
}

TEST(EngineArgs, EpsilonValid) {
    EXPECT_NO_THROW(runArgs({"--epsilon", "0.1"}));
}

TEST(EngineArgs, PayoffsValid) {
    EXPECT_NO_THROW(runArgs({"--payoffs", "10,8,3,1"}));
}

TEST(EngineArgs, StrategiesValid) {
    EXPECT_NO_THROW(runArgs({"--strategies", "ALLC,ALLD,TFT"}));
}

TEST(EngineArgs, FormatValid) {
    EXPECT_NO_THROW(runArgs({"--format", "json"}));
}

TEST(EngineArgs, EvolveFlag) {
    EXPECT_NO_THROW(runArgs({"--evolve"}));
}

TEST(EngineArgs, EvolveExplicitOne) {
    EXPECT_NO_THROW(runArgs({"--evolve", "1"}));
}

TEST(EngineArgs, PopulationValid) {
    EXPECT_NO_THROW(runArgs({"--population", "500"}));
}

TEST(EngineArgs, GenerationsValid) {
    EXPECT_NO_THROW(runArgs({"--generations", "200"}));
}

TEST(EngineArgs, MutationValid) {
    EXPECT_NO_THROW(runArgs({"--mutation", "0.05"}));
}

TEST(EngineArgs, SaveValid) {
    EXPECT_NO_THROW(runArgs({"--save", "config.txt"}));
}

TEST(EngineArgs, LoadValid) {
    // Write a valid config file
    const char* filename = "valid_config.txt";
    std::ofstream file(filename);
    file << "--rounds 200 --format json";
    file.close();

    EXPECT_NO_THROW(runArgs({"--load", filename}));
}

TEST(EngineArgs, LoadWithChainedArgs) {
    // Load config + append more args
    const char* filename = "chained_config.txt";
    std::ofstream file(filename);
    file << "--rounds 100";
    file.close();

    EXPECT_NO_THROW(runArgs({"--load", filename, "--format", "csv"}));
}

// --------------------------- INVALID CASES ---------------------------

TEST(EngineArgs, RoundsInvalid) {
    EXPECT_THROW(runArgs({"--rounds", "abc"}), std::invalid_argument);
}

TEST(EngineArgs, ArgumentInvalid) {
    EXPECT_THROW(runArgs({"--arg", "123"}), std::invalid_argument);
}

TEST(EngineArgs, PayoffsInvalidOrder) {
    EXPECT_THROW(runArgs({"--payoffs", "5,5,5,5"}), std::invalid_argument);
}

TEST(EngineArgs, PayoffsInvalidPolicy) {
    EXPECT_THROW(runArgs({"--payoffs", "10,8,3,10"}), std::invalid_argument);
}

TEST(EngineArgs, StrategyInvalid) {
    EXPECT_THROW(runArgs({"--strategies", "UNKNOWN"}), std::invalid_argument);
}

TEST(EngineArgs, FormatInvalid) {
    EXPECT_THROW(runArgs({"--format", "xml"}), std::invalid_argument);
}

TEST(EngineArgs, EvolveInvalidParam) {
    EXPECT_THROW(runArgs({"--evolve", "banana"}), std::invalid_argument);
}

TEST(EngineArgs, SaveInvalidFilename) {
    EXPECT_THROW(runArgs({"--save", "nofile"}), std::invalid_argument);
}

TEST(EngineArgs, SaveLoadConflict) {
    EXPECT_THROW(runArgs({"--save", "config.txt", "--load", "config.txt"}), std::invalid_argument);
}

TEST(EngineArgs, LoadFileDoesNotExist) {
    EXPECT_THROW(runArgs({"--load", "nonexistent.txt"}), std::invalid_argument);
}

TEST(EngineArgs, LoadThenParseInvalidInsideFile) {
    // Write an invalid config (bad rounds)
    const char* filename = "invalid_config.txt";
    std::ofstream file(filename);
    file << "--rounds abc";
    file.close();

    EXPECT_THROW(runArgs({"--load", filename}), std::invalid_argument);
}